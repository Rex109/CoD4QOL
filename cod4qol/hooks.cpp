#include "hooks.hpp"
#include "game.hpp"
#include <psapi.h>
#include <cassert>
#include "offsets.hpp"

void hooks::InitializeHooks()
{
	std::cout << "Initializing hooks..." << std::endl;

	//Removing cod4x safechecks
	hooks::write_addr(offsets::GetOffset("safechecks"), "\xC3", 1);

	//Fast startup
	hooks::write_addr(offsets::GetOffset("faststartup"), "\xC3", 1);

	//Console name
	game::pCon_LinePrefix = (game::Con_LinePrefix)(0x460613);
	hooks::install(&(PVOID&)game::pCon_LinePrefix, (PBYTE)game::hookedCon_LinePrefix);

	//Game start
	game::pDB_LoadXZoneFromGfxConfig = (game::DB_LoadXZoneFromGfxConfig)(offsets::GetOffset("DB_LoadXZoneFromGfxConfig"));
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
	game::pCG_Respawn = (game::CG_Respawn)(offsets::GetOffset("CG_Respawn"));
	hooks::install(&(PVOID&)game::pCG_Respawn, (PBYTE)game::hookedCG_Respawn);

	//VSTR
	game::pCmd_Vstr_f = (game::Cmd_Vstr_f)(0x4F9550);
	hooks::install(&(PVOID&)game::pCmd_Vstr_f, (PBYTE)game::hookedCmd_Vstr_f);

	//ScreenshotRequest
	game::pScreenshotRequest = (game::ScreenshotRequest)(offsets::GetOffset("ScreenshotRequest"));
	hooks::install(&(PVOID&)game::pScreenshotRequest, (PBYTE)game::hookedScreenshotRequest);

	//R_SetViewParmsForScene
	game::pR_SetViewParmsForScene = (game::R_SetViewParmsForScene)(0x5FAA05);
	hooks::install(&(PVOID&)game::pR_SetViewParmsForScene, (PBYTE)game::hookedR_SetViewParmsForScene);

	//R_DrawXmodelSkinnedCached
	game::pR_DrawXmodelSkinnedCached = (game::R_DrawXmodelSkinnedCached)(0x646870);
	hooks::install(&(PVOID&)game::pR_DrawXmodelSkinnedCached, (PBYTE)game::hookedR_DrawXmodelSkinnedCached);

	//R_DrawXModelRigidModelSurf
	game::pR_DrawXModelRigidModelSurf = (game::R_DrawXModelRigidModelSurf)(0x656974);
	hooks::install(&(PVOID&)game::pR_DrawXModelRigidModelSurf, (PBYTE)game::hookedR_DrawXModelRigidModelSurf);

	//R_DrawXModelRigidModelSurf_End
	game::pR_DrawXModelRigidModelSurf_End = (game::R_DrawXModelRigidModelSurf_End)(0x6569BD);
	hooks::install(&(PVOID&)game::pR_DrawXModelRigidModelSurf_End, (PBYTE)game::hookedR_DrawXModelRigidModelSurf_End);

	//CG_StartAmbient
	game::pCG_StartAmbient = (game::CG_StartAmbient)(0x43F200);
	hooks::install(&(PVOID&)game::pCG_StartAmbient, (PBYTE)game::hookedCG_StartAmbient);

	//CG_DObjGetLocalBoneMatrix
	game::pCG_DObjGetLocalBoneMatrix = (game::CG_DObjGetLocalBoneMatrix)(0x433F35);
	hooks::install(&(PVOID&)game::pCG_DObjGetLocalBoneMatrix, (PBYTE)game::hookedCG_DObjGetLocalBoneMatrix);

	std::cout << "Hooks installed!" << std::endl;
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