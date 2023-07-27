#include "hooks.hpp"
#include "game.hpp"

void hooks::InitializeHooks()
{
	//Console name
	game::pCon_LinePrefix = (game::Con_LinePrefix)(0x460613);
	hooks::install(&(PVOID&)game::pCon_LinePrefix, (PBYTE)game::hookedCon_LinePrefix);

	//Game start
	game::pDB_LoadXZoneFromGfxConfig = (game::DB_LoadXZoneFromGfxConfig)(game::cod4x_entry + 0x8327E);
	hooks::install(&(PVOID&)game::pDB_LoadXZoneFromGfxConfig, (PBYTE)game::hookedDB_LoadXZoneFromGfxConfig);

	//Return to main menu
	game::pCom_LoadUiFastFile = (game::Com_LoadUiFastFile)(0x500200);
	hooks::install(&(PVOID&)game::pCom_LoadUiFastFile, (PBYTE)game::hookedCom_LoadUiFastFile);

	//Ingame
	game::pCL_InitCGame = (game::CL_InitCGame)(0x5F50B0);
	hooks::install(&(PVOID&)game::pCL_InitCGame, (PBYTE)game::hookedCL_InitCGame);

	//Dvar registration
	game::pCL_RegisterDvars = (game::CL_RegisterDvars)(0x629D70);
	hooks::install(&(PVOID&)game::pCL_RegisterDvars, (PBYTE)game::hookedCL_RegisterDvars);

	//Respawn
	game::pCG_Respawn = (game::CG_Respawn)(game::cod4x_entry + 0x3B45);
	hooks::install(&(PVOID&)game::pCG_Respawn, (PBYTE)game::hookedCG_Respawn);

	//VSTR
	game::pCmd_Vstr_f = (game::Cmd_Vstr_f)(0x4F9550);
	hooks::install(&(PVOID&)game::pCmd_Vstr_f, (PBYTE)game::hookedCmd_Vstr_f);

	//ScreenshotRequest
	game::pScreenshotRequest = (game::ScreenshotRequest)(game::cod4x_entry + 0xEA610);
	hooks::install(&(PVOID&)game::pScreenshotRequest, (PBYTE)game::hookedScreenshotRequest);

	//Text_PaintCenter
	game::pText_PaintCenter = (game::Text_PaintCenter)(0x549A60);
	hooks::install(&(PVOID&)game::pText_PaintCenter, (PBYTE)game::hookedText_PaintCenter);
}

LONG hooks::install(void** ppPointer, void* pDetour)
{
	DetourTransactionBegin();
	DetourAttach(ppPointer, pDetour);
	return DetourTransactionCommit();
}

void hooks::write_addr(DWORD addr, const char* bytes, size_t len)
{
	DWORD oldProtect;

	VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)addr, bytes, len);
	VirtualProtect((LPVOID)addr, len, oldProtect, NULL);
}