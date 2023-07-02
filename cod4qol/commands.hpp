#pragma once
#include "game.hpp"
#include <iostream>
#include <fstream>

namespace commands
{
	inline game::dvar_s* cg_fovscale;
	inline game::dvar_s* r_fullbright;
	inline game::dvar_s* r_zfar;

	inline game::dvar_s* qol_getss;
	inline game::dvar_s* qol_vstr_block;

	void InitializeCommands();

	void LoadZone();
	void iPrintLnBold(const char* text);
	void WriteProtectedConfig();
	void ReadProtectedConfig();
}