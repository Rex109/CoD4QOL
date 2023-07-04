#include "commands.hpp"

void commands::InitializeCommands()
{
	game::Cmd_AddCommand("loadzone", LoadZone);
    game::Cmd_AddCommand("readprotectedconfig", ReadProtectedConfig);
    game::Cmd_AddCommand("writeprotectedconfig", WriteProtectedConfig);
    game::Cmd_AddCommand("toggleconsoleupdate", ToggleConsoleUpdate);

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

    protectedconf.close();
}

void commands::ToggleConsoleUpdate()
{
    ShowWindow(*game::hwnd, !strcmp(game::Find("qol_show_console")->current.string, "1") ? SW_SHOWNOACTIVATE : SW_HIDE);
}