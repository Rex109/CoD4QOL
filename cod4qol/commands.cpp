#include "commands.hpp"
#include "updater.hpp"
#include "hooks.hpp"
#include <filesystem>

void commands::InitializeCommands()
{
    std::cout << "Initializing commands..." << std::endl;

    game::Cmd_AddCommand("loadzone", LoadZone);
    game::Cmd_AddCommand("readprotectedconfig", ReadProtectedConfig);
    game::Cmd_AddCommand("writeprotectedconfig", WriteProtectedConfig);
    game::Cmd_AddCommand("toggleconsoleupdate", ToggleConsoleUpdate);
    game::Cmd_AddCommand("toggleloadinginfoupdate", ToggleLoadingInfoUpdate);
    game::Cmd_AddCommand("togglesteamauthupdate", ToggleSteamAuthUpdate);
    game::Cmd_AddCommand("vm_anim", VmAnim);

    game::Cmd_AddCommand("loaddemos", LoadDemos);
    game::Cmd_AddCommand("playselecteddemo", PlaySelectedDemo);
    game::Cmd_AddCommand("getdemoname", GetDemoName);
    game::Cmd_AddCommand("deleteselecteddemo", DeleteSelectedDemo);
    game::Cmd_AddCommand("renameselecteddemo", RenameSelectedDemo);

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

    cg_draw2d = game::Find("cg_draw2d");
    cg_draw2d->flags = game::none;

    cg_drawgun = game::Find("cg_drawgun");
    cg_drawgun->flags = game::none;

    sv_running = game::Find("sv_running");

    qol_check_updates = game::Dvar_RegisterBool("qol_check_updates", 1, game::dvar_flags::saved, "Enable cod4qol to check for updates at every startup.");

    static const char* qol_getss_names[] = { "Off", "Notify", "Notify + Block", NULL };
    qol_getss = game::Dvar_RegisterEnum("qol_getss", qol_getss_names, 0, game::dvar_flags::saved, "Notify and block getss from servers.");
    
    qol_vstr_block = game::Dvar_RegisterBool("qol_vstr_block", 0, game::dvar_flags::saved, "Block every forced client commands from mods.");

    qol_show_console = game::Dvar_RegisterBool("qol_show_console", 0, game::dvar_flags::saved, "Show the game's console on a separated window. In order to update it you need to execute \"toggleconsoleupdate\".");

    qol_show_loading = game::Dvar_RegisterBool("qol_show_loading", 1, game::dvar_flags::saved, "Show mod and map info during loading. In order to update it you need to execute \"toggleloadinginfoupdate\".");

    qol_mirrorgun = game::Dvar_RegisterBool("qol_mirrorgun", 0, game::dvar_flags::saved, "Flip the gun on the left side of the screen.");

    qol_disable_steam_auth = game::Dvar_RegisterBool("qol_disable_steam_auth", 0, game::dvar_flags::saved, "Disable the authentication with steam if it is running on the computer. In order to update it you need to execute \"togglesteamauthupdate\".");

    qol_ambient = game::Dvar_RegisterBool("qol_ambient", 1, game::dvar_flags::saved, "Enable ambient sound/music cues.");

    qol_stockmenu = game::Dvar_RegisterBool("qol_stockmenu", 0, game::dvar_flags::saved, "Load the stock menu even with a mod loaded.");

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

    std::cout << game::String_Alloc("Pingu") << std::endl;

    protectedconf.close();
}

void commands::ToggleConsoleUpdate()
{
    ShowWindow(*game::hwnd, qol_show_console->current.enabled ? SW_SHOWNOACTIVATE : SW_HIDE);
}

void commands::SetGun(game::GfxViewParms* view_parms)
{
    if (commands::qol_mirrorgun->current.enabled)
        view_parms->projectionMatrix.m[0][0] = -view_parms->projectionMatrix.m[0][0];
}

void commands::ToggleLoadingInfoUpdate()
{
    if (!commands::qol_show_loading->current.enabled)
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

void commands::ToggleSteamAuthUpdate()
{
    if (commands::qol_disable_steam_auth->current.enabled)
    {
        hooks::write_addr(game::cod4x_entry + 0x1A70A, "\x90\x90\x90\x90\x90\x90", 6);
        hooks::write_addr(game::cod4x_entry + 0x1A717, "\x90\x90\x90\x90\x90\x90", 6);
    }
    else
    {
        hooks::write_addr(game::cod4x_entry + 0x1A70A, "\x0F\x85\xDB\x00\x00\x00", 6);
        hooks::write_addr(game::cod4x_entry + 0x1A717, "\x0F\x85\xCE\x00\x00\x00", 6);
    }
}

void commands::LoadDemos()
{
    std::string relative_dir = getDemosFolder();

    *game::modCount = 0;
    *game::modIndex = 0;

    for (const auto& entry : std::filesystem::directory_iterator(relative_dir))
    {
        if (entry.path().extension() != ".dm_1")
			continue;

        const char* string = game::String_Alloc(entry.path().stem().string().c_str());

        game::modName[2 * (*game::modCount)] = string;
        game::modDesc[2 * (*game::modCount)] = string;

        (*game::modCount)++;
    }

    if((*game::modCount) < 1)
        game::Cmd_ExecuteSingleCommand(0, 0, "set qol_show_demos_buttons 0\n");
    else
        game::Cmd_ExecuteSingleCommand(0, 0, "set qol_show_demos_buttons 1\n");
}

void commands::PlaySelectedDemo()
{
    if(*game::modCount < 1)
        return;

    std::string cmd = "demo " + std::string(game::modName[2 * (*game::modIndex)]);

    game::Cbuf_AddText(cmd.c_str(), 0);
}

void commands::DeleteSelectedDemo()
{
    if (*game::modCount < 1)
		return;

	std::string relative_dir = getDemosFolder();
	relative_dir.append("\\" + std::string(game::modName[2 * (*game::modIndex)]) + ".dm_1");

    std::filesystem::remove(relative_dir);

	commands::LoadDemos();
}

void commands::RenameSelectedDemo()
{
    game::dvar_s* qol_rename_text = game::Find("qol_rename_text");

    if(!qol_rename_text)
		return;

    if (*game::modCount < 1)
		return;

    std::string relative_dir = getDemosFolder();
    relative_dir.append("\\" + std::string(game::modName[2 * (*game::modIndex)]) + ".dm_1");

    std::string new_name = relative_dir.substr(0, relative_dir.find_last_of("\\/")) + "\\" + std::string(qol_rename_text->current.string) + ".dm_1";

    if (!isValidDestinationPath(new_name))
        return;

    std::filesystem::rename(relative_dir, new_name);
}

void commands::GetDemoName()
{
    if (*game::modCount < 1)
		return;

    std::string cmd = "set qol_rename_text " + std::string(game::modName[2 * (*game::modIndex)]) + "\n";

    game::Cmd_ExecuteSingleCommand(0, 0, cmd.c_str());
}

std::string getDemosFolder()
{

    std::string relative_dir = game::fs_homepath->current.string;
    relative_dir.append("\\");
    relative_dir.append(!strcmp(game::fs_game->current.string, "") ? "main" : game::fs_game->current.string);
    relative_dir.append("\\demos");

    return relative_dir;
}

bool isValidDestinationPath(const std::filesystem::path& destination)
{
    const std::string disallowedChars = "<>:\"/\\|?*";

    for (char c : destination.filename().string())
        if (disallowedChars.find(c) != std::string::npos)
        {
            std::cout << "Invalid character '" << c << "' in the destination path." << std::endl;
            return false;
        }

    return true;
}