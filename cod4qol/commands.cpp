#include "commands.hpp"
#include "updater.hpp"
#include "hooks.hpp"
#include <filesystem>
#include "base64/base64.h"
#include <sstream>

void commands::InitializeCommands()
{
    std::cout << "Initializing commands..." << std::endl;

    game::Cmd_AddCommand("unlockall", UnlockAll);
    game::Cmd_AddCommand("loadzone", LoadZone);
    game::Cmd_AddCommand("loadiwd", LoadIWD);
    game::Cmd_AddCommand("vm_anim", VmAnim);
    game::Cmd_AddCommand("readprotectedconfig", ReadProtectedConfig);
    game::Cmd_AddCommand("writeprotectedconfig", WriteProtectedConfig);
    game::Cmd_AddCommand("toggleconsoleupdate", ToggleConsoleUpdate);
    game::Cmd_AddCommand("toggleloadinginfoupdate", ToggleLoadingInfoUpdate);
    game::Cmd_AddCommand("togglesteamauthupdate", ToggleSteamAuthUpdate);
    game::Cmd_AddCommand("toggleflashbangupdate", ToggleFlashbangUpdate);
    game::Cmd_AddCommand("openlink", OpenLink);

    game::Cmd_AddCommand("loaddemos", LoadDemos);
    game::Cmd_AddCommand("playselecteddemo", PlaySelectedDemo);
    game::Cmd_AddCommand("getdemoname", GetDemoName);
    game::Cmd_AddCommand("deleteselecteddemo", DeleteSelectedDemo);
    game::Cmd_AddCommand("renameselecteddemo", RenameSelectedDemo);

    game::Cmd_AddCommand("updatecod4qol", updater::Update);

    game::Cmd_AddCommand("crosshair_config", CrosshairConfig);

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

    cg_fov = game::Find("cg_fov");
    cg_fov->domain.value.min = 1;
    cg_fov->domain.value.max = 180;

    r_lodBiasRigid = game::Find("r_lodBiasRigid");
    r_lodBiasRigid->domain.value.min = -1000000;

    r_lodBiasSkinned = game::Find("r_lodBiasSkinned");
    r_lodBiasSkinned->domain.value.min = -1000000;

    qol_version = game::Cvar_RegisterString("qol_version", COD4QOL_VERSION, game::dvar_flags::read_only, "CoD4QOL version.");

    qol_show_mainmenuinfo = game::Cvar_RegisterBool("qol_show_mainmenuinfo", 1, game::dvar_flags::saved, "Show the CoD4QOL version and social media links in the main menu.");

    qol_check_updates = game::Cvar_RegisterBool("qol_check_updates", 1, game::dvar_flags::saved, "Enable cod4qol to check for updates at every startup.");

    qol_forceiwdextract = game::Cvar_RegisterBool("qol_forceiwdextract", 0, game::dvar_flags::saved, "Force the extraction of the iwd files at the next startup.");

    static const char* qol_getss_names[] = { "Off", "Notify", "Notify + Block", NULL };
    qol_getss = game::Cvar_RegisterEnum("qol_getss", qol_getss_names, 0, game::dvar_flags::saved, "Notify and block getss from servers.");
    
    qol_vstr_block = game::Cvar_RegisterBool("qol_vstr_block", 0, game::dvar_flags::saved, "Block every forced client commands from mods.");

    qol_show_console = game::Cvar_RegisterBool("qol_show_console", 0, game::dvar_flags::saved, "Show the game's console on a separated window. In order to update it you need to execute \"toggleconsoleupdate\".");

    qol_show_loading = game::Cvar_RegisterBool("qol_show_loading", 1, game::dvar_flags::saved, "Show mod and map info during loading. In order to update it you need to execute \"toggleloadinginfoupdate\".");

    qol_mirrorgun = game::Cvar_RegisterBool("qol_mirrorgun", 0, game::dvar_flags::saved, "Flip the gun on the left side of the screen.");

    qol_disable_steam_auth = game::Cvar_RegisterBool("qol_disable_steam_auth", 0, game::dvar_flags::saved, "Disable the authentication with steam if it is running on the computer. In order to update it you need to execute \"togglesteamauthupdate\".");

    qol_ambient = game::Cvar_RegisterBool("qol_ambient", 1, game::dvar_flags::saved, "Enable ambient sound/music cues.");

    qol_stockmenu = game::Cvar_RegisterBool("qol_stockmenu", 0, game::dvar_flags::saved, "Load the stock menu even with a mod loaded.");

    qol_enableautobhop = game::Cvar_RegisterBool("qol_enableautobhop", 0, game::dvar_flags::saved, "Enable auto bunny hopping.");

    qol_debugreflections = game::Cvar_RegisterBool("qol_debugreflections", 0, game::dvar_flags::saved, "Enable red reflections, useful for fixing reflections on custom maps. Requires map restart.");

    qol_renderscale = game::Cvar_RegisterFloat("qol_renderscale", 1.0f, 0.1f, 1.0f, game::dvar_flags::saved, "Scales the internal rendering resolution.");

    qol_disableslashcommands = game::Cvar_RegisterBool("qol_disableslashcommands", 0, game::dvar_flags::saved, "Executes commands in console without slashes.");

    qol_invertflashbang = game::Cvar_RegisterBool("qol_invertflashbang", 0, game::dvar_flags::saved, "Invert the flashbang effect. In order to update it you need to execute \"toggleflashbangupdate\".");

    qol_loadcustomlocalizedresources = game::Cvar_RegisterBool("qol_loadcustomlocalizedresources", 0, game::dvar_flags::saved, "Load custom localized resources from \\main\\custom_localized_resources.iwd.");

    qol_enablecustomcrosshair = game::Cvar_RegisterBool("qol_enablecustomcrosshair", 0, game::dvar_flags::saved, "Enable custom crosshair.");

    qol_customcrosshairsize = game::Cvar_RegisterInt("qol_customcrosshairsize", 5, 0, 5000, game::dvar_flags::saved, "Custom crosshair size.");

    qol_customcrosshairthickness = game::Cvar_RegisterInt("qol_customcrosshairthickness", 2, 0, 5000, game::dvar_flags::saved, "Custom crosshair thickness.");

    qol_customcrosshairgap = game::Cvar_RegisterInt("qol_customcrosshairgap", 5, 0, 5000, game::dvar_flags::saved, "Custom crosshair gap.");

    qol_customcrosshaircolor_r = game::Cvar_RegisterFloat("qol_customcrosshaircolor_r", 1.0f, 0.0f, 1.0f, game::dvar_flags::saved, "Custom crosshair color red component.");
    qol_customcrosshaircolor_g = game::Cvar_RegisterFloat("qol_customcrosshaircolor_g", 1.0f, 0.0f, 1.0f, game::dvar_flags::saved, "Custom crosshair color green component.");
    qol_customcrosshaircolor_b = game::Cvar_RegisterFloat("qol_customcrosshaircolor_b", 1.0f, 0.0f, 1.0f, game::dvar_flags::saved, "Custom crosshair color blue component.");
    qol_customcrosshaircolor_a = game::Cvar_RegisterFloat("qol_customcrosshaircolor_a", 1.0f, 0.0f, 1.0f, game::dvar_flags::saved, "Custom crosshair color alpha component.");

    qol_customcrosshairdot = game::Cvar_RegisterBool("qol_customcrosshairdot", 0, game::dvar_flags::saved, "Enable custom crosshair dot.");

    qol_customcrosshairspectrum = game::Cvar_RegisterBool("qol_customcrosshairspectrum", 0, game::dvar_flags::saved, "Enable custom crosshair color spectrum.");

    qol_customcrosshairoutlinethickness = game::Cvar_RegisterInt("qol_customcrosshairoutlinethickness", 0, 0, 5000, game::dvar_flags::saved, "Custom crosshair outline thickness.");

    qol_chatfontsize = game::Cvar_RegisterFloat("qol_chatfontsize", 10.0f, 0.0f, 100.0f, game::dvar_flags::saved, "Chat font size.");

    std::cout << "Commands initialized!" << std::endl;
}

void commands::UnlockAll()
{
    if (strcmp(game::fs_game->current.string, ""))
    {
        game::Com_PrintMessage(0, "You must be playing without a mod to use unlockall\n", 0);
        return;
	}

    auto SetValueAtIndex = [](int offset, int32_t value) {
        int index = offset / sizeof(int32_t);
        int32_t* ptr = reinterpret_cast<int32_t*>(0xCC18C90);
        ptr[index] = value;
    };

    SetValueAtIndex(0, 0);

    SetValueAtIndex(104, 16843008);
    SetValueAtIndex(152, 16842752);
    SetValueAtIndex(156, 16843009);
    SetValueAtIndex(160, 65793);
    SetValueAtIndex(164, 16843009);
    SetValueAtIndex(168, 16843009);
    SetValueAtIndex(176, 256);
    SetValueAtIndex(180, 1);
    SetValueAtIndex(188, 65793);
    SetValueAtIndex(192, 65536);
    SetValueAtIndex(252, 905969765);
    SetValueAtIndex(256, 54);
    SetValueAtIndex(260, 65792);
    SetValueAtIndex(264, 33554433);
    SetValueAtIndex(272, 16842752);
    SetValueAtIndex(276, 16843009);
    SetValueAtIndex(280, 16843009);
    SetValueAtIndex(300, 167772160);
    SetValueAtIndex(504, -256);
    SetValueAtIndex(508, -1);
    SetValueAtIndex(512, -1);
    SetValueAtIndex(516, 65535);
    SetValueAtIndex(524, -256);
    SetValueAtIndex(528, -1);
    SetValueAtIndex(532, 16777215);
    SetValueAtIndex(544, -256);
    SetValueAtIndex(548, 16777215);
    SetValueAtIndex(564, -256);
    SetValueAtIndex(568, 255);
    SetValueAtIndex(584, -256);
    SetValueAtIndex(588, -1);
    SetValueAtIndex(592, 16777215);
    SetValueAtIndex(604, -256);
    SetValueAtIndex(608, -1);
    SetValueAtIndex(612, -1);
    SetValueAtIndex(616, -1);
    SetValueAtIndex(620, 255);
    SetValueAtIndex(624, -256);
    SetValueAtIndex(628, -1);
    SetValueAtIndex(632, -1);
    SetValueAtIndex(636, 255);
    SetValueAtIndex(644, -256);
    SetValueAtIndex(648, -1);
    SetValueAtIndex(652, -1);
    SetValueAtIndex(656, -1);
    SetValueAtIndex(660, 255);
    SetValueAtIndex(664, -256);
    SetValueAtIndex(668, -1);
    SetValueAtIndex(672, -1);
    SetValueAtIndex(676, -1);
    SetValueAtIndex(680, -1);
    SetValueAtIndex(684, -1);
    SetValueAtIndex(688, -1);
    SetValueAtIndex(692, -1);
    SetValueAtIndex(696, -1);
    SetValueAtIndex(700, 16777215);
    SetValueAtIndex(756, 1);
    SetValueAtIndex(3208, 125490);
    SetValueAtIndex(3244, 62);
    SetValueAtIndex(3264, 1092);
    SetValueAtIndex(3268, 51);
    SetValueAtIndex(3272, 1233);
    SetValueAtIndex(3280, 1);
    SetValueAtIndex(3288, 41);
    SetValueAtIndex(3292, 2070);
    SetValueAtIndex(3296, 22388);
    SetValueAtIndex(3300, 24407);
    SetValueAtIndex(3304, 848);
    SetValueAtIndex(3404, 54);
    SetValueAtIndex(3408, 120280);
    SetValueAtIndex(3412, 125490);
    SetValueAtIndex(3416, 125490);
    SetValueAtIndex(3604, 3025);
    SetValueAtIndex(3608, 3004);
    SetValueAtIndex(3612, 3025);
    SetValueAtIndex(4008, 150);
    SetValueAtIndex(4012, 150);
    SetValueAtIndex(4016, 150);
    SetValueAtIndex(4020, 150);
    SetValueAtIndex(4024, 150);
    SetValueAtIndex(4028, 150);
    SetValueAtIndex(4032, 150);
    SetValueAtIndex(4036, 150);
    SetValueAtIndex(4040, 150);
    SetValueAtIndex(4044, 150);
    SetValueAtIndex(4048, 150);
    SetValueAtIndex(4052, 150);
    SetValueAtIndex(4056, 150);
    SetValueAtIndex(4088, 150);
    SetValueAtIndex(4092, 150);
    SetValueAtIndex(4096, 150);
    SetValueAtIndex(4100, 150);
    SetValueAtIndex(4104, 150);
    SetValueAtIndex(4108, 150);
    SetValueAtIndex(4112, 150);
    SetValueAtIndex(4116, 150);
    SetValueAtIndex(4120, 150);
    SetValueAtIndex(4124, 150);
    SetValueAtIndex(4168, 150);
    SetValueAtIndex(4172, 150);
    SetValueAtIndex(4176, 150);
    SetValueAtIndex(4180, 150);
    SetValueAtIndex(4184, 150);
    SetValueAtIndex(4188, 150);
    SetValueAtIndex(4248, 50);
    SetValueAtIndex(4252, 100);
    SetValueAtIndex(4256, 50);
    SetValueAtIndex(4260, 100);
    SetValueAtIndex(4328, 100);
    SetValueAtIndex(4332, 150);
    SetValueAtIndex(4336, 100);
    SetValueAtIndex(4340, 150);
    SetValueAtIndex(4344, 100);
    SetValueAtIndex(4348, 150);
    SetValueAtIndex(4352, 100);
    SetValueAtIndex(4356, 150);
    SetValueAtIndex(4360, 100);
    SetValueAtIndex(4364, 150);
    SetValueAtIndex(4408, 1);
    SetValueAtIndex(4412, 1);
    SetValueAtIndex(4416, 1);
    SetValueAtIndex(4420, 1);
    SetValueAtIndex(4424, 1661220);
    SetValueAtIndex(4428, 1);
    SetValueAtIndex(4432, 1);
    SetValueAtIndex(4436, 30);
    SetValueAtIndex(4440, 30);
    SetValueAtIndex(4444, 10);
    SetValueAtIndex(4448, 30);
    SetValueAtIndex(4452, 15);
    SetValueAtIndex(4456, 15);
    SetValueAtIndex(4460, 20);
    SetValueAtIndex(4464, 15);
    SetValueAtIndex(4468, 15);
    SetValueAtIndex(4488, 10);
    SetValueAtIndex(4492, 30);
    SetValueAtIndex(4496, 30);
    SetValueAtIndex(4500, 1);
    SetValueAtIndex(4504, 15);
    SetValueAtIndex(4508, 50);
    SetValueAtIndex(4512, 1);
    SetValueAtIndex(4516, 1);
    SetValueAtIndex(4520, 1);
    SetValueAtIndex(4524, 1);
    SetValueAtIndex(4528, 1);
    SetValueAtIndex(4532, 1);
    SetValueAtIndex(4568, 5);
    SetValueAtIndex(4572, 50);
    SetValueAtIndex(4576, 50);
    SetValueAtIndex(4580, 50);
    SetValueAtIndex(4584, 15);
    SetValueAtIndex(4588, 20);
    SetValueAtIndex(4592, 30);
    SetValueAtIndex(4596, 30);
    SetValueAtIndex(4600, 25);
    SetValueAtIndex(4604, 1);
    SetValueAtIndex(4608, 15);
    SetValueAtIndex(4612, 1);
    SetValueAtIndex(4616, 50);
    SetValueAtIndex(4620, 50);
    SetValueAtIndex(4624, 10);
    SetValueAtIndex(4628, 10);
    SetValueAtIndex(4648, 1);
    SetValueAtIndex(4652, 1);
    SetValueAtIndex(4656, 1);
    SetValueAtIndex(4660, 1);
    SetValueAtIndex(4664, 1);
    SetValueAtIndex(4668, 1);
    SetValueAtIndex(4672, 15);
    SetValueAtIndex(4676, 1);
    SetValueAtIndex(4680, 1);
    SetValueAtIndex(4684, 1);
    SetValueAtIndex(4688, 25);
    SetValueAtIndex(4692, 3);
    SetValueAtIndex(4696, 15);
    SetValueAtIndex(4700, 1);
    SetValueAtIndex(4704, 1);
    SetValueAtIndex(4708, 1);
    SetValueAtIndex(4712, 1);
    SetValueAtIndex(4716, 1);
    SetValueAtIndex(4720, 1);
    SetValueAtIndex(4724, 1);
    SetValueAtIndex(4728, 1);
    SetValueAtIndex(4732, 1);
    SetValueAtIndex(4736, 10);
    SetValueAtIndex(4740, 1);
    SetValueAtIndex(4744, 1);
    SetValueAtIndex(4748, 1);
    SetValueAtIndex(4752, 1);
    SetValueAtIndex(4756, 1);
    SetValueAtIndex(4760, 1);
    SetValueAtIndex(4764, 1);
    SetValueAtIndex(4768, 1);
    SetValueAtIndex(4772, 1);
    SetValueAtIndex(4776, 1);
    SetValueAtIndex(4780, 1);
    SetValueAtIndex(4784, 1);
    SetValueAtIndex(4788, 1);
    SetValueAtIndex(4792, 1);
    SetValueAtIndex(4796, 1);
    SetValueAtIndex(6004, 9);
    SetValueAtIndex(6008, 524297);
    SetValueAtIndex(6012, 9);
    SetValueAtIndex(6016, 1);
    SetValueAtIndex(6020, 1);
    SetValueAtIndex(6044, 7951);
    SetValueAtIndex(6048, 470687503);
    SetValueAtIndex(6052, 521019151);
    SetValueAtIndex(6056, 521019151);
    SetValueAtIndex(6060, 1057898255);
    SetValueAtIndex(6084, 16175);
    SetValueAtIndex(6088, 523116335);
    SetValueAtIndex(6092, 520101633);
    SetValueAtIndex(6096, 523116335);
    SetValueAtIndex(6100, 523116335);
    SetValueAtIndex(6104, 470687535);
    SetValueAtIndex(6108, 523116335);
    SetValueAtIndex(6244, 1057111811);
    SetValueAtIndex(6248, 7939);
    SetValueAtIndex(6252, 520232707);
    SetValueAtIndex(6260, 520232707);
    SetValueAtIndex(6264, 520232707);
    SetValueAtIndex(6284, 1058357013);
    SetValueAtIndex(6288, 471146261);
    SetValueAtIndex(6324, 471277335);
    SetValueAtIndex(6328, 471277335);
    SetValueAtIndex(6332, 1058488087);
    SetValueAtIndex(6668, 120280);
    SetValueAtIndex(6672, 54);
    SetValueAtIndex(6744, 12310346);
    SetValueAtIndex(6748, 16159304);
    SetValueAtIndex(6752, 121138);

	int* WriteFlag = reinterpret_cast<int*>(0xCC1AC91);
	*WriteFlag = 1;

    game::LiveStorage_UploadStats();
}

void commands::LoadZone()
{
    if (game::Cmd_Argc() < 2)
    {
        game::Com_PrintMessage(0, "Usage: loadzone <zoneName>\n", 0);
        return;
    }

    std::string relative_dir = game::fs_homepath->current.string;
    relative_dir.append("\\zone\\");
    relative_dir.append(game::localization);
    relative_dir.append("\\");
    relative_dir.append(game::Cmd_Argv(1));
    relative_dir.append(".ff");

    std::cout << relative_dir << std::endl;

    if (!std::filesystem::exists(relative_dir))
    {
		game::Com_PrintMessage(0, "Zone file not found\n", 0);
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

void commands::LoadIWD()
{
    if (!game::sv_cheats->current.enabled)
    {
        game::Com_PrintMessage(0, "You must be playing with cheats enabled to use loadiwd\n", 0);
        return;
    }

    if (game::Cmd_Argc() < 2)
    {
        game::Com_PrintMessage(0, "Usage: loadiwd <iwdName>\n", 0);
        return;
    }

    std::string iwd_name = game::Cmd_Argv(1);

    std::string relative_dir = game::fs_homepath->current.string;
    relative_dir.append("\\main\\");
    relative_dir.append(iwd_name);

    game::FS_AddSingleIwdFileForGameDirectory(relative_dir.c_str(), iwd_name.c_str(), "main");;
}

void commands::VmAnim()
{
    if (!game::sv_cheats->current.enabled)
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
    const static HWND* hwnd = reinterpret_cast<HWND*>(offsets::GetOffset("hwnd"));

    ShowWindow(*hwnd, qol_show_console->current.enabled ? SW_SHOWNOACTIVATE : SW_HIDE);
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
    static const DWORD steam_auth_a = offsets::GetOffset("steam_auth_a");
    static const DWORD steam_auth_b = offsets::GetOffset("steam_auth_b");

    static const offsets::data_t steam_auth_a_bytes = offsets::GetData("steam_auth_a");
    static const offsets::data_t steam_auth_b_bytes = offsets::GetData("steam_auth_b");

    static const offsets::data_t steam_auth_a_bytes_original = offsets::GetData("steam_auth_a_original");
    static const offsets::data_t steam_auth_b_bytes_original = offsets::GetData("steam_auth_b_original");


    if (commands::qol_disable_steam_auth->current.enabled)
    {
        hooks::write_addr(steam_auth_a, steam_auth_a_bytes.data.c_str(), steam_auth_a_bytes.size);
        hooks::write_addr(steam_auth_b, steam_auth_b_bytes.data.c_str(), steam_auth_b_bytes.size);
    }
    else
    {
        hooks::write_addr(steam_auth_a, steam_auth_a_bytes_original.data.c_str(), steam_auth_a_bytes_original.size);
        hooks::write_addr(steam_auth_b, steam_auth_b_bytes_original.data.c_str(), steam_auth_b_bytes_original.size);
    }
}

void commands::ToggleFlashbangUpdate()
{
    if (commands::qol_invertflashbang->current.enabled)
		game::rgp->shellShockFlashedMaterial = game::blackMaterial;
    else
		game::rgp->shellShockFlashedMaterial = game::flashMaterial;
}

void commands::OpenLink()
{
    if (game::Cmd_Argc() < 2)
    {
        game::Com_PrintMessage(0, "Usage: openlink <linkid>\n", 0);
        return;
    }

    int linkId = std::atoi(game::Cmd_Argv(1));
    std::string link;

    switch (linkId)
    {
        case 0:
            link = "https://discord.com/invite/QDYk75vBBk";
            break;
        case 1:
            link = "https://www.youtube.com/@_rex109_";
            break;
        case 2:
            link = "https://github.com/Rex109";
            break;
        case 3:
            link = "https://ko-fi.com/rex109";
            break;
        default:
            return;
    }

    ShellExecute(0, 0, link.c_str(), 0, 0, SW_SHOW);
}

void commands::LoadDemos()
{
    std::string relative_dir = getDemosFolder();

    *game::modCount = 0;
    *game::modIndex = 0;

    if (std::filesystem::exists(relative_dir))
    {
        for (const auto& entry : std::filesystem::directory_iterator(relative_dir))
        {
            if (entry.path().extension() != ".dm_1")
                continue;

            const char* string = game::String_Alloc(entry.path().stem().string().c_str());

            game::modName[2 * (*game::modCount)] = string;
            game::modDesc[2 * (*game::modCount)] = string;

            (*game::modCount)++;
        }
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

    std::string cmd = "demo \"" + std::string(game::modName[2 * (*game::modIndex)]) + "\"";

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

    std::string new_name = qol_rename_text->current.string;

    if (!isValidDestinationName(new_name))
        return;

    std::string new_path = relative_dir.substr(0, relative_dir.find_last_of("\\/")) + "\\" + new_name + ".dm_1";
    std::filesystem::rename(relative_dir, new_path);
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

bool isValidDestinationName(const std::string destination)
{
    const std::string disallowedChars = "<>:\"/\\|?*";

    for (char c : destination)
        if (disallowedChars.find(c) != std::string::npos)
        {
            std::cout << "Invalid character '" << c << "' in the destination path." << std::endl;
            return false;
        }

    return true;
}

void commands::CrosshairConfig()
{
    if (game::Cmd_Argc() < 2)
    {
        game::Com_PrintMessage(0, "Usage: crosshair_config <mode>\n", 0);
        return;
    }

    std::string mode = game::Cmd_Argv(1);

    if (mode == "0")
    {
        std::string crosshair_config = "VERSION 1.0";
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshairsize->current.integer));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshairthickness->current.integer));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshairgap->current.integer));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshairoutlinethickness->current.integer));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshairdot->current.enabled));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshaircolor_r->current.value));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshaircolor_g->current.value));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshaircolor_b->current.value));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshaircolor_a->current.value));
        crosshair_config.append("\n");
        crosshair_config.append(std::to_string(qol_customcrosshairspectrum->current.enabled));

        std::string base64 = base64_encode(reinterpret_cast<const unsigned char*>(crosshair_config.c_str()), crosshair_config.length());
		const char* output = base64.c_str();

        if (OpenClipboard(0))
        {
			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, strlen(output) + 1);
            if (hg)
            {
				memcpy(GlobalLock(hg), output, strlen(output) + 1);
				GlobalUnlock(hg);
				SetClipboardData(CF_TEXT, hg);
			}

			CloseClipboard();
			GlobalFree(hg);
		}
	}
    else if (mode == "1")
    {
        if (!IsClipboardFormatAvailable(CF_TEXT))
        {
			game::Com_PrintMessage(0, "No text data in clipboard.\n", 0);
			return;
		}

        if (OpenClipboard(0))
        {
			HGLOBAL hg = GetClipboardData(CF_TEXT);
            if (hg)
            {
				char* output = static_cast<char*>(GlobalLock(hg));
				std::string base64 = output;
				GlobalUnlock(hg);

                try {
                    std::string crosshair_config = base64_decode(base64);
                    std::istringstream iss(crosshair_config);
                    std::string line;

                    std::getline(iss, line);
                    if (line != "VERSION 1.0")
                    {
                        game::Com_PrintMessage(0, "Invalid crosshair config version.\n", 0);
                        CloseClipboard();
                        return;
                    }

                    std::getline(iss, line);
                    qol_customcrosshairsize->current.integer = std::stoi(line);
                    qol_customcrosshairsize->latched.integer = std::stoi(line);

                    std::getline(iss, line);
                    qol_customcrosshairthickness->current.integer = std::stoi(line);
                    qol_customcrosshairthickness->latched.integer = std::stoi(line);

                    std::getline(iss, line);
                    qol_customcrosshairgap->current.integer = std::stoi(line);
                    qol_customcrosshairgap->latched.integer = std::stoi(line);

                    std::getline(iss, line);
                    qol_customcrosshairoutlinethickness->current.integer = std::stoi(line);
                    qol_customcrosshairoutlinethickness->latched.integer = std::stoi(line);

                    std::getline(iss, line);
                    qol_customcrosshairdot->current.enabled = std::stoi(line);
                    qol_customcrosshairdot->latched.enabled = std::stoi(line);

                    std::getline(iss, line);
                    qol_customcrosshaircolor_r->current.value = std::stof(line);
                    qol_customcrosshaircolor_r->latched.value = std::stof(line);

                    std::getline(iss, line);
                    qol_customcrosshaircolor_g->current.value = std::stof(line);
                    qol_customcrosshaircolor_g->latched.value = std::stof(line);

                    std::getline(iss, line);
                    qol_customcrosshaircolor_b->current.value = std::stof(line);
                    qol_customcrosshaircolor_b->latched.value = std::stof(line);

                    std::getline(iss, line);
                    qol_customcrosshaircolor_a->current.value = std::stof(line);
                    qol_customcrosshaircolor_a->latched.value = std::stof(line);

                    std::getline(iss, line);
                    qol_customcrosshairspectrum->current.enabled = std::stoi(line);
                    qol_customcrosshairspectrum->latched.enabled = std::stoi(line);

                    *game::dvar_modifiedFlags |= 1u;
                }
                catch (const std::exception& e)
                {
					game::Com_PrintMessage(0, "Invalid crosshair config.\n", 0);
				}

                CloseClipboard();
			}
		}
    }
}