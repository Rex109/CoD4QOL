#include "game.hpp"
#include "commands.hpp"
#include "hooks.hpp"

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
	startup = false;

	game::Sys_CreateConsole(0x0);

	if (!strcmp(fs_game->current.string, ""))
		game::Cmd_ExecuteSingleCommand(0, 0, "loadzone qol\n");

	commands::InitializeCommands();

	game::Cmd_ExecuteSingleCommand(0, 0, "toggleconsoleupdate\n");
	game::Cmd_ExecuteSingleCommand(0, 0, "readprotectedconfig\n");
	
	return game::pDB_LoadXZoneFromGfxConfig();
}

void game::hookedCom_LoadUiFastFile()
{
	if (!strcmp(fs_game->current.string, ""))
		game::Cmd_ExecuteSingleCommand(0, 0, "loadzone qol\n");
		
	return game::pCom_LoadUiFastFile();
}

void game::hookedCL_InitCGame()
{
	game::Cmd_ExecuteSingleCommand(0, 0, "loadzone qol\n");

	return game::pCL_InitCGame();
}

void game::hookedCL_RegisterDvars()
{
	if (startup)
		return game::pCL_RegisterDvars();
}

void game::hookedCG_Respawn()
{
	game::Cmd_ExecuteSingleCommand(0, 0, "readprotectedconfig\n");

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