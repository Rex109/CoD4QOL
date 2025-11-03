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

	//Remove localized IWD restrictions
	if (offsets::GetCRC32() == COD4QOL_COD4X_CRC32_212)
	{
		hooks::write_addr(game::cod4x_entry + 0x30E10, "\x90\x90\x90\x90\x90\x90", 6);
		hooks::write_addr(game::cod4x_entry + 0x30E1D, "\xEB", 1);
	}
	else if (offsets::GetCRC32() == COD4QOL_COD4X_CRC32_211)
		hooks::write_addr(game::cod4x_entry + 0x3A953, "\xE9\x88\x00\x00\x00\x90", 6);

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

	//CL_CmdButtons
	game::pCL_CmdButtons = (game::CL_CmdButtons)(0x4639C8);
	hooks::install(&(PVOID&)game::pCL_CmdButtons, (PBYTE)game::hookedCL_CmdButtons);

	//CL_Disconnect
	game::pCL_Disconnect = (game::CL_Disconnect)(0x4696B0);
	hooks::install(&(PVOID&)game::pCL_Disconnect, (PBYTE)game::hookedCL_Disconnect);

	//RB_DrawDebugPostEffects
	game::pRB_DrawDebugPostEffects = (game::RB_DrawDebugPostEffects)(0x64AD70);
	hooks::install(&(PVOID&)game::pRB_DrawDebugPostEffects, (PBYTE)game::hookedRB_DrawDebugPostEffects);

	//RB_DebugShaderDrawCommandsCommon
	game::pRB_DebugShaderDrawCommandsCommon = (game::RB_DebugShaderDrawCommandsCommon)(0x649DD7);
	hooks::install(&(PVOID&)game::pRB_DebugShaderDrawCommandsCommon, (PBYTE)game::hookedRB_DebugShaderDrawCommandsCommon);

	//R_GenerateSortedDrawSurfs
	game::pR_GenerateSortedDrawSurfs = (game::R_GenerateSortedDrawSurfs)(0x5F98E0);
	hooks::install(&(PVOID&)game::pR_GenerateSortedDrawSurfs, (PBYTE)game::hookedR_GenerateSortedDrawSurfs);

	//Console_Key
	game::pConsole_Key = (game::Console_Key)(0x46752F);
	hooks::install(&(PVOID&)game::pConsole_Key, (PBYTE)game::hookedConsole_Key);

	//R_RecoverLostDevice
	game::pR_RecoverLostDevice = (game::R_RecoverLostDevice)(0x5F5360);
	hooks::install(&(PVOID&)game::pR_RecoverLostDevice, (PBYTE)game::hookedR_RecoverLostDevice);

	//R_RecoverLostDevice_End
	game::pR_RecoverLostDevice_End = (game::R_RecoverLostDevice_End)(0x5F5526);
	hooks::install(&(PVOID&)game::pR_RecoverLostDevice_End, (PBYTE)game::hookedR_RecoverLostDevice_End);

	//CG_DrawCrosshair
	game::pCG_DrawCrosshair = (game::CG_DrawCrosshair)(0x4311A0);
	hooks::install(&(PVOID&)game::pCG_DrawCrosshair, (PBYTE)game::hookedCG_DrawCrosshair);

	//Give
	game::pCmd_Give_f = (game::Cmd_Give_f)(0x4B27EC);
	hooks::install(&(PVOID&)game::pCmd_Give_f, (PBYTE)game::hookedCmd_Give_f);

	//CG_DrawChatMessages
	game::pCG_DrawChatMessages = (game::CG_DrawChatMessages)(0x42D141);
	hooks::install(&(PVOID&)game::pCG_DrawChatMessages, (PBYTE)game::hookedCG_DrawChatMessages);

	//UpdateShellShockSound
	game::pUpdateShellShockSound = (game::UpdateShellShockSound)(0x44CD49);
	hooks::install(&(PVOID&)game::pUpdateShellShockSound, (PBYTE)game::hookedUpdateShellShockSound);

	//CG_SetClientDvarFromServer
	game::pCG_SetClientDvarFromServer = (game::CG_SetClientDvarFromServer)(0x44BD90);
	hooks::install(&(PVOID&)game::pCG_SetClientDvarFromServer, (PBYTE)game::hookedCG_SetClientDvarFromServer);

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