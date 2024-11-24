#include "game.hpp"
#include "commands.hpp"
#include "hooks.hpp"
#include "updater.hpp"
#include "resource.h"
#include "offsets.hpp"
#include <crc32/crc32.h>

__declspec(naked) const char* game::hookedCon_LinePrefix()
{
	const static uint32_t retn_addr = 0x460618;

	__asm
	{
		mov		eax, prefix;
		jmp		retn_addr;
	}
}

HMODULE game::GetCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModule, &hModule);

	return hModule;
}

bool game::LoadLocalizedIWD(const char* pakfile, const char* basename, const char* gamename)
{
	static const DWORD iwd_flag = offsets::GetOffset("iwd_flag");

	hooks::write_addr(iwd_flag, "\x01", 1);
	bool result = FS_AddSingleIwdFileForGameDirectory(pakfile, basename, gamename);
	hooks::write_addr(iwd_flag, "\x00", 1);

	return result;
}

void game::LoadModFiles()
{
	std::cout << "Loading mod files..." << std::endl;

	std::string relative_dir = game::fs_homepath->current.string;
	relative_dir.append("\\main\\xcommon_cod4qol.iwd");

	if (startup && (!std::filesystem::exists(relative_dir) || commands::qol_forceiwdextract->current.enabled))
	{
		HRSRC hRes = FindResource(game::GetCurrentModule(), MAKEINTRESOURCE(COD4QOL_IWD), RT_RCDATA);

		if (!hRes)
		{
			std::cout << "Failed to find resource: " << GetLastError() << std::endl;
			return;
		}

		HGLOBAL hData = LoadResource(game::GetCurrentModule(), hRes);


		if (!hData)
		{
			std::cout << "Failed to load resource: " << GetLastError() << std::endl;
			return;
		}

		DWORD size = SizeofResource(game::GetCurrentModule(), hRes);
		byte* data = (byte*)LockResource(hData);

		game::WriteBytesToFile(data, size, relative_dir.c_str());

		UnlockResource(data);
		FreeResource(hData);

		if(!commands::qol_forceiwdextract->current.enabled)
			std::cout << "Resource extracted to file: " << relative_dir << std::endl;
		else
		{
			std::cout << "Resource updated to file: " << relative_dir << std::endl;
			commands::qol_forceiwdextract->current.enabled = false;
			commands::qol_forceiwdextract->latched.enabled = false;
		}
	}

	LoadLocalizedIWD(relative_dir.c_str(), "xcommon_cod4qol.iwd", "main");
	game::Cbuf_AddText("loadzone qol\n", 0);
}

void game::WriteBytesToFile(const byte* data, DWORD size, const char* filename)
{
	std::ofstream outfile(filename, std::ios::binary);
	if (!outfile) {
		std::cerr << "Error opening file for writing: " << filename << std::endl;
		return;
	}

	outfile.write(reinterpret_cast<const char*>(data), size);
	outfile.close();

	std::cout << "Bytes written to file: " << filename << std::endl;
}

void game::cleanUpReflections()
{
	int removed = 0;
	for (int i = 0; i < rgp->world->reflectionProbeCount; i++)
	{
		D3DLOCKED_RECT lockedRect;
		IDirect3DCubeTexture9* cubemap = rgp->world->reflectionProbes[i].reflectionImage->texture.cubemap;

		cubemap->LockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0, &lockedRect, NULL, 0);

		CRC32 crc32;
		std::string hash = crc32(lockedRect.pBits, lockedRect.Pitch * lockedRect.Pitch);
		cubemap->UnlockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0);

		if (hash == "e06ccbba" || hash == "444c60df" || hash == "c1eacd74" || hash == "48ed9648")
		{
			rgp->world->reflectionProbes[i].reflectionImage->texture.cubemap = nullptr;
			removed++;
		}
	}

	std::cout << "Removed " << removed << " reflection probes." << std::endl;
}

void game::hookedDB_LoadXZoneFromGfxConfig()
{
	std::cout << "Calling DB_LoadXZoneFromGfxConfig();" << std::endl;

	game::Sys_CreateConsole(0x0);

	commands::InitializeCommands();

	if (!strcmp(fs_game->current.string, "") || commands::qol_stockmenu->current.enabled)
		LoadModFiles();

	if (startup)
	{
		#ifndef _DEBUG
			if (commands::qol_check_updates->current.enabled)
				updater::CheckForUpdates();
		#endif

		startup = false;
	}

	game::Cbuf_AddText("toggleconsoleupdate\n", 0);
	game::Cbuf_AddText("toggleloadinginfoupdate\n", 0);
	game::Cbuf_AddText("togglesteamauthupdate\n", 0);
	game::Cbuf_AddText("readprotectedconfig\n", 0);
	
	return game::pDB_LoadXZoneFromGfxConfig();
}

void game::hookedCom_LoadUiFastFile()
{
	std::cout << "Calling Com_LoadUiFastFile();" << std::endl;

	if (!strcmp(fs_game->current.string, "") || commands::qol_stockmenu->current.enabled)
		LoadModFiles();
		
	return game::pCom_LoadUiFastFile();
}

void game::hookedCL_InitCGame()
{
	std::cout << "Calling CL_InitCGame();" << std::endl;

	if (!commands::qol_debugreflections->current.enabled)
	{
		std::cout << "Cleaning up reflections..." << std::endl;
		game::cleanUpReflections();
	}

	LoadModFiles();

	return game::pCL_InitCGame();
}

void game::hookedCL_RegisterDvars()
{
	std::cout << "Calling CL_RegisterDvars();" << std::endl;

	if (startup)
		return game::pCL_RegisterDvars();
}

void game::hookedCG_Respawn()
{
	std::cout << "Calling CG_Respawn();" << std::endl;

	game::Cbuf_AddText("readprotectedconfig\n", 0);

	return game::pCG_Respawn();
}

void game::hookedCmd_Vstr_f()
{
	std::cout << "Received vstr" << std::endl;

	if (!commands::qol_vstr_block->current.enabled)
		return game::pCmd_Vstr_f();
}

int game::hookedScreenshotRequest(int a1, int a2)
{
	static const DWORD ss_switch = offsets::GetOffset("ss_switch");

	std::cout << "Received screenshot request" << std::endl;

	if (commands::qol_getss->current.integer != 0)
		commands::iPrintLnBold("[^3CoD4QOL^7]: ^1You are currently being screenshotted");

	if (commands::qol_getss->current.integer == 2)
		hooks::write_addr(ss_switch, "\xEB", 1);
	else
		hooks::write_addr(ss_switch, "\x74", 1);

	return pScreenshotRequest(a1, a2);
}

__declspec(naked) void game::hookedR_SetViewParmsForScene()
{
	const static uint32_t retn_addr = 0x5FAA0B;
	__asm
	{
		pushad;
		push	edi;
		call	commands::SetGun;
		add		esp, 4;
		popad;

		lea     ecx, [edi + 0C0h];
		jmp		retn_addr;
	}
}

void game::hookedR_DrawXmodelSkinnedCached(int a1, int a2, int a3)
{
	if (commands::qol_mirrorgun->current.enabled)
	{
		(*game::dx9_device_ptr)->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		game::pR_DrawXmodelSkinnedCached(a1, a2, a3);
		(*game::dx9_device_ptr)->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		return;
	}

	return game::pR_DrawXmodelSkinnedCached(a1, a2, a3);
}

__declspec(naked) void game::hookedR_DrawXModelRigidModelSurf()
{
	__asm pushad;

	if (commands::qol_mirrorgun->current.enabled)
		(*game::dx9_device_ptr)->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	__asm
	{
		popad;
		jmp game::pR_DrawXModelRigidModelSurf;
	}
}

__declspec(naked) void game::hookedR_DrawXModelRigidModelSurf_End()
{
	__asm pushad;

	if (commands::qol_mirrorgun->current.enabled)
		(*game::dx9_device_ptr)->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__asm
	{
		popad;
		jmp game::pR_DrawXModelRigidModelSurf_End;
	}
}

unsigned int game::hookedCG_StartAmbient(int a1)
{
	std::cout << "Calling CG_StartAmbient();" << std::endl;

	if (!commands::qol_ambient->current.enabled)
		return 0;

	return game::pCG_StartAmbient(a1);
}

__declspec(naked) void game::hookedCL_CmdButtons()
{
	static bool jump_pressed = false;
	const static uint32_t retn_addr = 0x4639CF;
	__asm pushad;

	if (!jump_pressed || !commands::qol_enableautobhop->current.enabled)
	{
		jump_pressed = true;
		__asm
		{
			popad;
			jmp game::pCL_CmdButtons;
		}
	}

	jump_pressed = false;
	__asm
	{
		popad;
		jmp retn_addr;
	}
}

int	game::Cmd_Argc()
{
	return game::cmd_args->argc[game::cmd_args->nesting];
}

const char* game::Cmd_Argv(int arg)
{
	return *(game::cmd_args->argv[game::cmd_args->nesting] + arg);
}

__declspec(naked) game::dvar_s* game::Find(const char*)
{
	__asm
	{
		push	eax;
		pushad;

		mov		edi, [esp + 28h];
		mov		eax, 0x56B5D0;
		call	eax;

		mov[esp + 20h], eax;
		popad;

		pop eax;
		retn;
	}
}

game::cmd_function_s* game::Cmd_AddCommand(const char* cmdname, void(__cdecl* function)())
{
	return ((game::cmd_function_s * (__cdecl*)(const char* cmd, void* function))game::Cmd_AddCommand_fnc)(cmdname, function);
}

void game::Cbuf_AddText(const char* text, int localClientNum)
{
	__asm {
		mov eax, text;
		push eax;
		mov ecx, localClientNum;
		push ecx;
		mov esi, 0x4F8D90;
		call esi;
		add esp, 0x8;
	}
}

__declspec(naked) const char* game::String_Alloc(const char* string)
{
	const static uint32_t retn_addr = 0x558E40;

	__asm
	{
		mov eax, [esp + 4];
		jmp retn_addr;
	}
}

void game::SetCoD4xFunctionOffsets()
{
	Cmd_AddCommand_fnc = (void*)(offsets::GetOffset("Cmd_AddCommand_fnc"));
	Sys_CreateConsole = Sys_CreateConsole_t(offsets::GetOffset("Sys_CreateConsole"));
	Cvar_RegisterBool = Cvar_RegisterBool_t(offsets::GetOffset("Cvar_RegisterBool"));
	Cvar_RegisterEnum = Cvar_RegisterEnum_t(offsets::GetOffset("Cvar_RegisterEnum"));
	Cvar_RegisterString = Cvar_RegisterString_t(offsets::GetOffset("Cvar_RegisterString"));
	FS_AddSingleIwdFileForGameDirectory = FS_AddSingleIwdFileForGameDirectory_t(offsets::GetOffset("FS_AddSingleIwdFileForGameDirectory"));
}