#include "commands.hpp"

void commands::InitializeCommands()
{
	game::Cmd_AddCommand("loadzone", LoadZone);
    game::Cmd_AddCommand("readprotectedconfig", ReadProtectedConfig);
    game::Cmd_AddCommand("writeprotectedconfig", WriteProtectedConfig);

    cg_fovscale = game::Find("cg_fovscale");
    cg_fovscale->flags = game::none;

    r_fullbright = game::Find("r_fullbright");
    r_fullbright->flags = game::none;

    r_zfar = game::Find("r_zfar");
    r_zfar->flags = game::none;

    qol_getss = game::Find("qol_getss");
    if (!qol_getss)
    {
        game::Cbuf_AddText("seta qol_getss 0\n", 0);
        qol_getss = game::Find("qol_getss");
    }
        
    qol_vstr_block = game::Find("qol_vstr_block");
    if (!qol_vstr_block)
    {
        game::Cbuf_AddText("seta qol_vstr_block 0\n", 0);
        qol_vstr_block = game::Find("qol_vstr_block");
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

    protectedconf.close();
}