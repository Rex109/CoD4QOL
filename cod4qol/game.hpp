#pragma once
#include <cstdint>
#include <Windows.h>
#include <iostream>
#include "defines.hpp"

namespace game
{
	enum dvar_flags : std::uint16_t
	{
		none = 0x0,
		saved = 0x1,
		user_info = 0x2,
		server_info = 0x4,
		replicated = 0x8,
		write_protected = 0x10,
		latched = 0x20,
		read_only = 0x40,
		cheat_protected = 0x80,
		temp = 0x100,
		no_restart = 0x400,
		user_created = 0x4000,
	};

	enum class dvar_type : std::int8_t
	{
		boolean = 0,
		value = 1,
		vec2 = 2,
		vec3 = 3,
		vec4 = 4,
		integer = 5,
		enumeration = 6,
		string = 7,
		color = 8,
		rgb = 9
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char* string;
		char color[4];
	};

	union DvarLimits
	{
		struct
		{
			int stringCount;
			const char** strings;
		} enumeration;

		struct
		{
			int min;
			int max;
		} integer;

		struct
		{
			float min;
			float max;
		} value;

		struct
		{
			float min;
			float max;
		} vector;
	};

	struct dvar_s
	{
		const char* name;
		const char* description;
		dvar_flags flags;
		dvar_type type;
		bool modified;
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarLimits domain;
		bool(__cdecl* domainFunc)(dvar_s*, DvarValue);
		dvar_s* hashNext;
	};

	struct cmd_function_s
	{
		cmd_function_s* next;
		const char* name;
		const char* args;
		const char* description;
		void(__cdecl* function)();
	};

	struct CmdArgs
	{
		int nesting;
		int localClientNum[8];
		int controllerIndex[8];
		int argc[8];
		const char** argv[8];
	};

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	inline bool startup = true;

	const static DWORD cod4x_entry = (DWORD)GetModuleHandleA("cod4x_021.dll");
	const static HWND* hwnd = reinterpret_cast<HWND*>(game::cod4x_entry + 0x43FE9A0);
	const static char* prefix = COD4QOL_CONSOLE_PREFIX;

	typedef const char*(*Con_LinePrefix)();
	inline Con_LinePrefix pCon_LinePrefix;

	const char* hookedCon_LinePrefix();

	typedef void(*DB_LoadXZoneFromGfxConfig)();
	inline DB_LoadXZoneFromGfxConfig pDB_LoadXZoneFromGfxConfig;

	void hookedDB_LoadXZoneFromGfxConfig();

	typedef void(*Com_LoadUiFastFile)();
	inline Com_LoadUiFastFile pCom_LoadUiFastFile;

	void hookedCom_LoadUiFastFile();

	typedef void(*CL_InitCGame)();
	inline CL_InitCGame pCL_InitCGame;

	void hookedCL_InitCGame();

	typedef void(*CL_RegisterDvars)();
	inline CL_RegisterDvars pCL_RegisterDvars;

	void hookedCL_RegisterDvars();

	typedef void(*CG_Respawn)();
	inline CG_Respawn pCG_Respawn;

	void hookedCG_Respawn();

	typedef void(*Cmd_Vstr_f)();
	inline Cmd_Vstr_f pCmd_Vstr_f;

	void hookedCmd_Vstr_f();

	typedef int(*ScreenshotRequest)(int a1, int a2);
	inline ScreenshotRequest pScreenshotRequest;

	int hookedScreenshotRequest(int a1, int a2);

	int	Cmd_Argc();
	const char* Cmd_Argv(int arg);

	dvar_s* Find(const char*);
	cmd_function_s* Cmd_AddCommand(const char* cmdname, void(__cdecl* function)());
	void Cbuf_AddText(const char* text, int localClientNum);

	typedef void(*Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
		extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void(*Com_PrintMessage_t)(int, const char*, char);
		extern Com_PrintMessage_t Com_PrintMessage;

	typedef void(*DB_LoadXAssets_t)(XZoneInfo* zoneInfo, unsigned int zone_count, int sync);
		extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef void(*Sys_CreateConsole_t)(int);
		extern Sys_CreateConsole_t Sys_CreateConsole;

	inline void* Cmd_AddCommand_fnc = (void*)(cod4x_entry + 0x2116C);
	inline game::CmdArgs* cmd_args = reinterpret_cast<game::CmdArgs*>(0x1410B40);

	inline game::Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x4F9AB0);
	inline game::Com_PrintMessage_t Com_PrintMessage = game::Com_PrintMessage_t(0x4FCA50);
	inline game::DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x48A2B0);
	inline game::Sys_CreateConsole_t Sys_CreateConsole = Sys_CreateConsole_t(cod4x_entry + 0x7F503);
}
