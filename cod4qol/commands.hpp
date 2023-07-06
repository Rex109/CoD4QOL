#pragma once
#include "game.hpp"
#include <iostream>
#include <fstream>

namespace commands
{
	inline game::dvar_s* sv_running;

	inline game::dvar_s* cg_fovscale;
	inline game::dvar_s* r_fullbright;
	inline game::dvar_s* r_zfar;
	inline game::dvar_s* r_filmusetweaks;
	inline game::dvar_s* developer_script;

	inline game::dvar_s* qol_getss;
	inline game::dvar_s* qol_vstr_block;
	inline game::dvar_s* qol_show_console;

	void InitializeCommands();

	void LoadZone();
	void iPrintLnBold(const char* text);
	void WriteProtectedConfig();
	void ReadProtectedConfig();
	void ToggleConsoleUpdate();
	void VmAnim();
}