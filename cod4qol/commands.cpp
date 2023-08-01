#include "commands.hpp"
#include "updater.hpp"
#include "hooks.hpp"

void commands::InitializeCommands()
{
    std::cout << "Initializing commands..." << std::endl;

	game::Cmd_AddCommand("loadzone", LoadZone);
    game::Cmd_AddCommand("readprotectedconfig", ReadProtectedConfig);
    game::Cmd_AddCommand("writeprotectedconfig", WriteProtectedConfig);
    game::Cmd_AddCommand("toggleconsoleupdate", ToggleConsoleUpdate);
    game::Cmd_AddCommand("toggleloadinginfoupdate", ToggleLoadingInfoUpdate);
    game::Cmd_AddCommand("vm_anim", VmAnim);

    game::Cmd_AddCommand("updatecod4qol", updater::Update);

    cg_fovscale = game::Find("cg_fovscale");
    cg_fovscale->flags = game::none;

    r_fullbright = game::Find("r_fullbright");
    r_fullbright->flags = game::none;

    r_zfar = game::Find("r_zfar");
    r_zfar->flags = game::none;

    r_filmusetweaks = game::Find("r_filmusetweaks");
    r_filmusetweaks->flags = game::none;

    developer_script = game::Find("developer_script");
    developer_script->flags = game::none;

    cg_gun_x = game::Find("cg_gun_x");
    cg_gun_x->flags = game::none;

    cg_gun_y = game::Find("cg_gun_y");
    cg_gun_y->flags = game::none;

    cg_gun_z = game::Find("cg_gun_z");
    cg_gun_z->flags = game::none;

    sv_running = game::Find("sv_running");

    qol_getss = game::Find("qol_getss");
    if (!qol_getss)
    {
        game::Cmd_ExecuteSingleCommand(0, 0, "seta qol_getss 0\n");
        qol_getss = game::Find("qol_getss");
    }
        
    qol_vstr_block = game::Find("qol_vstr_block");
    if (!qol_vstr_block)
    {
        game::Cmd_ExecuteSingleCommand(0, 0, "seta qol_vstr_block 0\n");
        qol_vstr_block = game::Find("qol_vstr_block");
    }

    qol_show_console = game::Find("qol_show_console");
    if (!qol_show_console)
    {
        game::Cmd_ExecuteSingleCommand(0, 0, "seta qol_show_console 0\n");
        qol_show_console = game::Find("qol_show_console");
    }

    qol_show_loading = game::Find("qol_show_loading");
    if (!qol_show_loading)
    {
        game::Cmd_ExecuteSingleCommand(0, 0, "seta qol_show_loading 1\n");
        qol_show_loading = game::Find("qol_show_loading");
    }

    qol_mirrorgun = game::Find("qol_mirrorgun");
    if (!qol_mirrorgun)
    {
        game::Cmd_ExecuteSingleCommand(0, 0, "seta qol_mirrorgun 0\n");
        qol_mirrorgun = game::Find("qol_mirrorgun");
    }

    std::cout << "Commands initialized!" << std::endl;
}

void commands::LoadZone()
{
    if (game::Cmd_Argc() < 2)
    {
        game::Com_PrintMessage(0, "Usage: loadzone <zoneName>\n", 0);
        return;
    }   

    game::XZoneInfo info[2];
    std::string zone = game::Cmd_Argv(1);

    info[0].name = 0;
    info[0].allocFlags = 0x0;
    info[0].freeFlags = 0x40;

    info[1].name = zone.data();
    info[1].allocFlags = 0x40;
    info[1].freeFlags = 0x0;

    game::DB_LoadXAssets(info, 2, 1);
}

void commands::VmAnim()
{
    if (!sv_running->current.enabled)
    {
        game::Com_PrintMessage(0, "You must be playing on a local server to use vm_anim\n", 0);
        return;
    }

    if (game::Cmd_Argc() < 2)
    {
        game::Com_PrintMessage(0, "Usage: vm_anim <index (integer from 0 to 32)>\n", 0);
        return;
    }

    int weaponIndex = std::atoi(game::Cmd_Argv(1));

    game::g_clients->ps.weaponstate = 0x0;
    game::g_clients->ps.weaponTime = 0;
    int pm_flags = game::g_clients->ps.pm_flags;
    if ((pm_flags & 1) != 0)
        game::g_clients->ps.pm_flags = pm_flags | 0x200;
    if (game::g_clients->ps.pm_type < 4)
        game::g_clients->ps.weapAnim = weaponIndex | ~(unsigned __int16)game::g_clients->ps.weapAnim & 0x200;
}

void commands::iPrintLnBold(const char* text)
{
    __asm
    {
        push text
        push 0
        mov esi, 0x43de00
        call esi
        add esp, 0x8
    }
}

void commands::ReadProtectedConfig()
{
    game::Cbuf_AddText("exec protected.cfg\n", 0);
}

void commands::WriteProtectedConfig()
{
    TCHAR path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);

    strcat_s(path, "\\main\\protected.cfg");

    std::ofstream protectedconf;

    protectedconf.open(path);

    protectedconf << "set" << " cg_fovscale \"" << cg_fovscale->current.value << "\"" << std::endl;
    protectedconf << "set" << " r_fullbright \"" << r_fullbright->current.enabled << "\"" << std::endl;
    protectedconf << "set" << " r_zfar \"" << r_zfar->current.value << "\"" << std::endl;
    protectedconf << "set" << " r_filmusetweaks \"" << r_filmusetweaks->current.enabled << "\"" << std::endl;
    protectedconf << "set" << " cg_gun_x \"" << cg_gun_x->current.value << "\"" << std::endl;
    protectedconf << "set" << " cg_gun_y \"" << cg_gun_y->current.value << "\"" << std::endl;
    protectedconf << "set" << " cg_gun_z \"" << cg_gun_z->current.value << "\"" << std::endl;

    protectedconf.close();
}

void commands::ToggleConsoleUpdate()
{
    ShowWindow(*game::hwnd, !strcmp(game::Find("qol_show_console")->current.string, "1") ? SW_SHOWNOACTIVATE : SW_HIDE);
}

void commands::SetGun(game::GfxViewParms* view_parms)
{
    if (!strcmp(commands::qol_mirrorgun->current.string, "1"))
        view_parms->projectionMatrix.m[0][0] = -view_parms->projectionMatrix.m[0][0];
}

void commands::ToggleLoadingInfoUpdate()
{
    if (!strcmp(commands::qol_show_loading->current.string, "0"))
    {
        hooks::write_addr(0x54A6B6, "\x90\x90\x90\x90\x90", 5); //Gametype
        hooks::write_addr(0x54A6FC, "\x90\x90\x90\x90\x90", 5); //Mapname
        hooks::write_addr(0x54A990, "\x90\x90\x90\x90\x90", 5); //Modname
    }
    else
    {
        hooks::write_addr(0x54A6B6, "\xE8\xA5\xF3\xFF\xFF", 5); //Gametype
        hooks::write_addr(0x54A6FC, "\xE8\x5F\xF3\xFF\xFF", 5); //Mapname
        hooks::write_addr(0x54A990, "\xE8\xCB\xF0\xFF\xFF", 5); //Modname
    }
}