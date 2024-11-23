#pragma once
#include "game.hpp"
#include <iostream>
#include <fstream>

namespace commands
{
	inline game::dvar_s* cg_fovscale;
	inline game::dvar_s* r_fullbright;
	inline game::dvar_s* r_zfar;
	inline game::dvar_s* r_filmusetweaks;
	inline game::dvar_s* developer_script;
	inline game::dvar_s* cg_gun_x;
	inline game::dvar_s* cg_gun_y;
	inline game::dvar_s* cg_gun_z;
	inline game::dvar_s* cg_draw2d;
	inline game::dvar_s* cg_drawgun;

	inline game::dvar_s* qol_rename_text;
	
	inline game::dvar_s* qol_version;
	inline game::dvar_s* qol_forceiwdextract;
	inline game::dvar_s* qol_check_updates;
	inline game::dvar_s* qol_getss;
	inline game::dvar_s* qol_vstr_block;
	inline game::dvar_s* qol_show_console;
	inline game::dvar_s* qol_show_loading;
	inline game::dvar_s* qol_mirrorgun;
	inline game::dvar_s* qol_disable_steam_auth;
	inline game::dvar_s* qol_ambient;
	inline game::dvar_s* qol_stockmenu;
	inline game::dvar_s* qol_enableautobhop;
	inline game::dvar_s* qol_show_mainmenuinfo;
	inline game::dvar_s* qol_debugreflections;

	void InitializeCommands();

	void LoadZone();
	void LoadIWD();
	void iPrintLnBold(const char* text);
	void WriteProtectedConfig();
	void ReadProtectedConfig();
	void ToggleConsoleUpdate();
	void VmAnim();
	void SetGun(game::GfxViewParms* view_parms);
	void ToggleLoadingInfoUpdate();
	void ToggleSteamAuthUpdate();
	void OpenLink();

	void LoadDemos();
	void PlaySelectedDemo();
	void DeleteSelectedDemo();
	void RenameSelectedDemo();
	void GetDemoName();
}

std::string getDemosFolder();
bool isValidDestinationName(const std::string destination);