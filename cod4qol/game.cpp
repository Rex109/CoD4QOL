#include "game.hpp"
#include "commands.hpp"
#include "hooks.hpp"
#include "updater.hpp"

game::dvar_s* fs_game = game::Find("fs_game");

__declspec(naked) const char* game::hookedCon_LinePrefix()
{
	const static uint32_t retn_addr = 0x460618;

	__asm
	{
		mov		eax, prefix;
		jmp		retn_addr;
	}
}

void game::hookedDB_LoadXZoneFromGfxConfig()
{
	std::cout << "Calling DB_LoadXZoneFromGfxConfig();" << std::endl;

	game::Sys_CreateConsole(0x0);

	if (!strcmp(fs_game->current.string, ""))
		game::Cbuf_AddText("loadzone qol\n", 0);

	commands::InitializeCommands();

	if (startup)
	{
		#ifndef _DEBUG
				if (!strcmp(commands::qol_check_updates->current.string, "1"))
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

	if (!strcmp(fs_game->current.string, ""))
		game::Cbuf_AddText("loadzone qol\n", 0);
		
	return game::pCom_LoadUiFastFile();
}

void game::hookedCL_InitCGame()
{
	std::cout << "Calling CL_InitCGame();" << std::endl;

	game::Cbuf_AddText("loadzone qol\n", 0);

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

	if (!strcmp(commands::qol_vstr_block->current.string, "0"))
		return game::pCmd_Vstr_f();
}

int game::hookedScreenshotRequest(int a1, int a2)
{
	std::cout << "Received screenshot request" << std::endl;

	if (strcmp(commands::qol_getss->current.string, "0"))
		commands::iPrintLnBold("[^3CoD4QOL^7]: ^1You are currently being screenshotted");

	if (!strcmp(commands::qol_getss->current.string, "2"))
		hooks::write_addr(game::cod4x_entry + 0xEA62B, "\xEB", 1);
	else
		hooks::write_addr(game::cod4x_entry + 0xEA62B, "\x74", 1);

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
	if (!strcmp(commands::qol_mirrorgun->current.string, "1"))
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

	if (!strcmp(commands::qol_mirrorgun->current.string, "1"))
	{
		(*game::dx9_device_ptr)->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	__asm
	{
		popad;
		jmp game::pR_DrawXModelRigidModelSurf;
	}
}

__declspec(naked) void game::hookedR_DrawXModelRigidModelSurf_End()
{
	__asm pushad;

	if (!strcmp(commands::qol_mirrorgun->current.string, "1"))
	{
		(*game::dx9_device_ptr)->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	__asm
	{
		popad;
		jmp game::pR_DrawXModelRigidModelSurf_End;
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