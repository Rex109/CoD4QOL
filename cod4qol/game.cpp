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
	if (startup)
	{
		#ifndef _DEBUG
			updater::CheckForUpdates();
		#endif

		startup = false;
	}

	game::Sys_CreateConsole(0x0);

	if (!strcmp(fs_game->current.string, ""))
		game::Cbuf_AddText("loadzone qol\n", 0);

	commands::InitializeCommands();

	game::Cbuf_AddText("toggleconsoleupdate\n", 0);
	game::Cbuf_AddText("readprotectedconfig\n", 0);
	
	return game::pDB_LoadXZoneFromGfxConfig();
}

void game::hookedCom_LoadUiFastFile()
{
	if (!strcmp(fs_game->current.string, ""))
		game::Cbuf_AddText("loadzone qol\n", 0);
		
	return game::pCom_LoadUiFastFile();
}

void game::hookedCL_InitCGame()
{
	game::Cbuf_AddText("loadzone qol\n", 0);

	return game::pCL_InitCGame();
}

void game::hookedCL_RegisterDvars()
{
	if (startup)
		return game::pCL_RegisterDvars();
}

void game::hookedCG_Respawn()
{
	game::Cbuf_AddText("readprotectedconfig\n", 0);

	return game::pCG_Respawn();
}

void game::hookedCmd_Vstr_f()
{
	if (!strcmp(commands::qol_vstr_block->current.string, "0"))
		return game::pCmd_Vstr_f();
}

int game::hookedScreenshotRequest(int a1, int a2)
{
	if (strcmp(commands::qol_getss->current.string, "0"))
		commands::iPrintLnBold("[^3CoD4QOL^7]: ^1You are currently being screenshotted");

	if (!strcmp(commands::qol_getss->current.string, "2"))
		hooks::write_addr(game::cod4x_entry + 0xEA62B, "\xEB", 1);
	else
		hooks::write_addr(game::cod4x_entry + 0xEA62B, "\x74", 1);

	return pScreenshotRequest(a1, a2);
}

__declspec(naked) void game::hookedText_PaintCenter()
{
	if (!strcmp(commands::qol_show_loading->current.string, "0"))
		__asm ret;
		
	__asm jmp game::pText_PaintCenter;
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