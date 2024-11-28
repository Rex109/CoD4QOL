#include "game.hpp"
#include "commands.hpp"
#include "hooks.hpp"
#include "updater.hpp"
#include "resource.h"
#include "offsets.hpp"
#include <crc32/crc32.h>

std::vector<std::pair<int, IDirect3DCubeTexture9*>> oldReflectionProbes;

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
	
	static int* iwd_flag_localized = reinterpret_cast<int*>(offsets::GetOffset("iwd_flag_localized"));
	static int* iwd_flag_lang = reinterpret_cast<int*>(offsets::GetOffset("iwd_flag_lang"));
	static dvar_s* loc_language = game::Find("loc_language");

	static DWORD oldProtect;
	static bool iwd_flag_localized_protect = VirtualProtect(iwd_flag_localized, sizeof(int*), PAGE_EXECUTE_READWRITE, &oldProtect);
	static bool iwd_flag_lang_protect = VirtualProtect(iwd_flag_lang, sizeof(int*), PAGE_EXECUTE_READWRITE, &oldProtect);

	*iwd_flag_localized = 1;
	*iwd_flag_lang = loc_language->current.integer;
	bool result = FS_AddSingleIwdFileForGameDirectory(pakfile, basename, gamename);
	*iwd_flag_localized = 0;
	*iwd_flag_lang = 0;

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
			std::cout << rgp->world->reflectionProbes[i].reflectionImage->texture.cubemap << std::endl;
			oldReflectionProbes.push_back(std::make_pair(i, cubemap));
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

void game::hookedCL_Disconnect(int localClientNum)
{
	if(oldReflectionProbes.empty())
		return game::pCL_Disconnect(localClientNum);

	std::cout << "Restoring old reflection probes..." << std::endl;

	for (auto& probe : oldReflectionProbes)
		rgp->world->reflectionProbes[probe.first].reflectionImage->texture.cubemap = probe.second;

	oldReflectionProbes.clear();

	return game::pCL_Disconnect(localClientNum);
}

void R_SetRenderTarget(int target)
{
	const static uint32_t R_SetRenderTarget_func = 0x632B60;
	const static uint32_t _gfxCmdBufSourceState = 0xD53F5F0;
	const static uint32_t _gfxCmdBufSourceSource = 0xD5404F0;

	__asm
	{
		pushad;
		push	_gfxCmdBufSourceSource;
		push	_gfxCmdBufSourceState;
		mov     eax, target;

		call	R_SetRenderTarget_func;
		add     esp, 8;
		popad;
	}
}

void R_Set2D()
{
	const static uint32_t R_Set2D_func = 0x6336E0;
	const static uint32_t _gfxCmdBufSourceState = 0xD53F5F0;

	__asm
	{
		pushad;
		mov		edi, [_gfxCmdBufSourceState];
		call	R_Set2D_func;
		popad;
	}
}

void RB_DrawStretchPic(game::Material* material, float x, float y, float w, float h, float texcoord0, float texcoord1, float texcoord2, float texcoord3 /*-1 pushed*/)
{
	const static uint32_t RB_DrawStretchPic_func = 0x610E10;
	__asm
	{
		pushad;
		mov		eax, material;
		push    0FFFFFFFFh;
		sub     esp, 20h;

		fld		texcoord3;
		fstp[esp + 1Ch];

		fld		texcoord2;
		fstp[esp + 18h];

		fld		texcoord1;
		fstp[esp + 14h];

		fld		texcoord0;
		fstp[esp + 10h];

		fld		h;
		fstp[esp + 0Ch];

		fld		w;
		fstp[esp + 8h];

		fld		y;
		fstp[esp + 4h];

		fld		x;
		fstp[esp];

		call	RB_DrawStretchPic_func;
		add     esp, 24h;
		popad;
	}
}

void applyFsr1(int a1)
{
	float renderscale = commands::qol_renderscale->current.value;

	if (renderscale != 1.0)
	{
		/*R_Set2D();
		R_SetRenderTarget(game::GfxRenderTargetId::R_RENDERTARGET_RESOLVED_POST_SUN);

		if (const auto material = game::rgp->postFxColorMaterial; material)
		{
			RB_DrawStretchPic(material, 0.0f, 0.0f, game::scrPlace->realViewableMax[0], game::scrPlace->realViewableMax[1], 0.0, 0.0, 1.0, 1.0);
		}

		game::RB_EndTessSurface();

		R_SetRenderTarget(game::GfxRenderTargetId::R_RENDERTARGET_FRAME_BUFFER);*/

		float centerX = 0.5f;
		float centerY = 0.5f;

		float halfWidth = 0.5f * renderscale;
		float halfHeight = 0.5f * renderscale;

		float s0 = centerX - halfWidth;
		float t0 = centerY - halfHeight;
		float s1 = centerX + halfWidth;
		float t1 = centerY + halfHeight;

		const auto postfx_fsr1 = game::Material_RegisterHandle("postfx_fsr1", 3);
		game::RB_DrawFullScreenColoredQuad(postfx_fsr1, s0, t0, s1, t1, -1);
	}
}

__declspec(naked) void game::hookedRB_DrawDebugPostEffects()
{
	const static uint32_t retn_addr = 0x64AD75;

	__asm
	{
		pushad;
		push	esi;
		call	applyFsr1;
		add		esp, 4;
		popad;

		jmp		retn_addr;
	}
}

char game::hookedR_GenerateSortedDrawSurfs(GfxSceneParms* sceneParams, int a2, int a3)
{
	float renderscale = commands::qol_renderscale->current.value;

	sceneParams->sceneViewport.width *= renderscale;
	sceneParams->sceneViewport.height *= renderscale;

	sceneParams->sceneViewport.x = (game::scrPlace->realViewableMax[0] - game::scrPlace->realViewableMax[0] * renderscale) / 2;
	sceneParams->sceneViewport.y = (game::scrPlace->realViewableMax[1] - game::scrPlace->realViewableMax[1] * renderscale) / 2;

	return game::pR_GenerateSortedDrawSurfs(sceneParams, a2, a3);
}

__declspec(naked) void game::hookedConsole_Key()
{
	const static uint32_t retn_addr = 0x467534;

	__asm pushad;

	if(commands::qol_disableslashcommands->current.enabled)
		__asm
		{
			popad;
			jmp retn_addr;
		}

	__asm
	{
	original:
		popad;
		jmp game::pConsole_Key;
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
	Cvar_RegisterFloat = Cvar_RegisterFloat_t(offsets::GetOffset("Cvar_RegisterFloat"));
	FS_AddSingleIwdFileForGameDirectory = FS_AddSingleIwdFileForGameDirectory_t(offsets::GetOffset("FS_AddSingleIwdFileForGameDirectory"));
}