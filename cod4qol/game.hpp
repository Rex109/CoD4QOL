#pragma once
#include <cstdint>
#include <Windows.h>
#include <iostream>
#include "defines.hpp"
#include <d3d9.h>

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

	enum OffhandSecondaryClass
	{
		PLAYER_OFFHAND_SECONDARY_SMOKE = 0x0,
		PLAYER_OFFHAND_SECONDARY_FLASH = 0x1,
		PLAYER_OFFHAND_SECONDARIES_TOTAL = 0x2,
	};

	enum ViewLockTypes
	{
		PLAYERVIEWLOCK_NONE = 0x0,
		PLAYERVIEWLOCK_FULL = 0x1,
		PLAYERVIEWLOCK_WEAPONJITTER = 0x2,
		PLAYERVIEWLOCKCOUNT = 0x3,
	};

	enum ActionSlotType
	{
		ACTIONSLOTTYPE_DONOTHING = 0x0,
		ACTIONSLOTTYPE_SPECIFYWEAPON = 0x1,
		ACTIONSLOTTYPE_ALTWEAPONTOGGLE = 0x2,
		ACTIONSLOTTYPE_NIGHTVISION = 0x3,
		ACTIONSLOTTYPECOUNT = 0x4,
	};

	struct ActionSlotParam_SpecifyWeapon
	{
		unsigned int index;
	};

	struct ActionSlotParam
	{
		ActionSlotParam_SpecifyWeapon specifyWeapon;
	};

	struct SprintState
	{
		int sprintButtonUpRequired;
		int sprintDelay;
		int lastSprintStart;
		int lastSprintEnd;
		int sprintStartMaxLength;
	};

	struct MantleState
	{
		float yaw;
		int timer;
		int transIndex;
		int flags;
	};

	enum objectiveState_t
	{
		OBJST_EMPTY = 0x0,
		OBJST_ACTIVE = 0x1,
		OBJST_INVISIBLE = 0x2,
		OBJST_DONE = 0x3,
		OBJST_CURRENT = 0x4,
		OBJST_FAILED = 0x5,
		OBJST_NUMSTATES = 0x6,
	};

	struct objective_t
	{
		objectiveState_t state;
		float origin[3];
		int entNum;
		int teamNum;
		int icon;
	};

	typedef enum
	{
		HE_TYPE_FREE = 0x0,
		HE_TYPE_TEXT = 0x1,
		HE_TYPE_VALUE = 0x2,
		HE_TYPE_PLAYERNAME = 0x3,
		HE_TYPE_MAPNAME = 0x4,
		HE_TYPE_GAMETYPE = 0x5,
		HE_TYPE_MATERIAL = 0x6,
		HE_TYPE_TIMER_DOWN = 0x7,
		HE_TYPE_TIMER_UP = 0x8,
		HE_TYPE_TENTHS_TIMER_DOWN = 0x9,
		HE_TYPE_TENTHS_TIMER_UP = 0xA,
		HE_TYPE_CLOCK_DOWN = 0xB,
		HE_TYPE_CLOCK_UP = 0xC,
		HE_TYPE_WAYPOINT = 0xD,
		HE_TYPE_COUNT = 0xE,
	}he_type_t;

	typedef struct
	{
		char r;
		char g;
		char b;
		char a;
	}hudelem_colorsplit_t;

	typedef union
	{
		hudelem_colorsplit_t split;
		int rgba;
	}hudelem_color_t;

	typedef struct hudelem_s
	{
		he_type_t type;
		float x;
		float y;
		float z;
		int targetEntNum;
		float fontScale;
		int font;
		int alignOrg;
		int alignScreen;
		hudelem_color_t color;
		hudelem_color_t fromColor; //0x28
		int fadeStartTime; //0x2c
		int fadeTime;
		int label;
		int width;
		int height; //0x3C
		int materialIndex;
		int offscreenMaterialIdx; //0x44
		int fromWidth;
		int fromHeight;
		int scaleStartTime;
		int scaleTime;
		float fromX;
		float fromY;
		int fromAlignOrg;
		int fromAlignScreen;
		int moveStartTime;
		int moveTime;
		int time;
		int duration;
		float value;
		int text;
		float sort;
		hudelem_color_t glowColor; //0x84
		int fxBirthTime;
		int fxLetterTime;
		int fxDecayStartTime;
		int fxDecayDuration;
		int soundID;
		int flags;
	}hudelem_t;

	typedef struct hudElemState_s
	{
		hudelem_t current[31];
		hudelem_t archival[31];
	}hudElemState_t;

	typedef struct playerState_s
	{
		int commandTime;
		int pm_type;
		int bobCycle;
		int pm_flags;
		int weapFlags;
		int otherFlags;
		int pm_time;
		float origin[3];
		float velocity[3];
		float oldVelocity[2];
		int weaponTime;
		int weaponDelay;
		int grenadeTimeLeft;
		int throwBackGrenadeOwner;
		int throwBackGrenadeTimeLeft;
		int weaponRestrictKickTime;
		int foliageSoundTime;
		int gravity;
		float leanf;
		int speed;
		float delta_angles[3];
		int groundEntityNum;
		float vLadderVec[3];
		int jumpTime;
		float jumpOriginZ;
		int legsTimer;
		int legsAnim;
		int torsoTimer;
		int torsoAnim;
		int legsAnimDuration;
		int torsoAnimDuration;
		int damageTimer;
		int damageDuration;
		int flinchYawAnim;
		int movementDir;
		int eFlags;
		int eventSequence;
		int events[4];
		unsigned int eventParms[4];
		int oldEventSequence;
		int clientNum;
		int offHandIndex;
		OffhandSecondaryClass offhandSecondary;
		unsigned int weapon;
		int weaponstate;
		unsigned int weaponShotCount;
		float fWeaponPosFrac;
		int adsDelayTime;
		int spreadOverride;
		int spreadOverrideState;
		int viewmodelIndex;
		float viewangles[3];
		int viewHeightTarget;
		float viewHeightCurrent;
		int viewHeightLerpTime;
		int viewHeightLerpTarget;
		int viewHeightLerpDown;
		float viewAngleClampBase[2];
		float viewAngleClampRange[2];
		int damageEvent;
		int damageYaw;
		int damagePitch;
		int damageCount;
		int stats[5];
		int ammo[128];
		int ammoclip[128];
		unsigned int weapons[4];
		unsigned int weaponold[4];
		unsigned int weaponrechamber[4];
		float proneDirection;
		float proneDirectionPitch;
		float proneTorsoPitch;
		ViewLockTypes viewlocked;
		int viewlocked_entNum;
		int cursorHint;
		int cursorHintString;
		int cursorHintEntIndex;
		int iCompassPlayerInfo;
		int radarEnabled;
		int locationSelectionInfo;
		SprintState sprintState;
		float fTorsoPitch;
		float fWaistPitch;
		float holdBreathScale;
		int holdBreathTimer;
		float moveSpeedScaleMultiplier;
		MantleState mantleState;
		float meleeChargeYaw;
		int meleeChargeDist;
		int meleeChargeTime;
		int perks;
		ActionSlotType actionSlotType[4];
		ActionSlotParam actionSlotParam[4];
		int entityEventSequence;
		int weapAnim;
		float aimSpreadScale;
		int shellshockIndex;
		int shellshockTime;
		int shellshockDuration;
		float dofNearStart;
		float dofNearEnd;
		float dofFarStart;
		float dofFarEnd;
		float dofNearBlur;
		float dofFarBlur;
		float dofViewmodelStart;
		float dofViewmodelEnd;
		int hudElemLastAssignedSoundID;
		objective_t objective[16];
		char weaponmodels[128];
		int deltaTime;
		int killCamEntity;
		hudElemState_t hud;
	}playerState_t;

	typedef enum
	{
		DEMO_TYPE_NONE = 0x0,
		DEMO_TYPE_CLIENT = 0x1,
		DEMO_TYPE_SERVER = 0x2,
	}DemoType;

	typedef enum
	{
		CUBEMAPSHOT_NONE = 0x0,
		CUBEMAPSHOT_RIGHT = 0x1,
		CUBEMAPSHOT_LEFT = 0x2,
		CUBEMAPSHOT_BACK = 0x3,
		CUBEMAPSHOT_FRONT = 0x4,
		CUBEMAPSHOT_UP = 0x5,
		CUBEMAPSHOT_DOWN = 0x6,
		CUBEMAPSHOT_COUNT = 0x7,
	}CubemapShot;

	struct gclient_s {
		playerState_s ps;
	};

	struct GfxMatrix
	{
		float m[4][4];
	};

	typedef struct
	{
		GfxMatrix viewMatrix;
		GfxMatrix projectionMatrix;
		GfxMatrix viewProjectionMatrix;
		GfxMatrix inverseViewProjectionMatrix;
		float origin[4];
		float axis[3][3];
		float depthHackNearClip;
		float zNear;
		float zFar;
	}GfxViewParms;

	inline bool startup = true;

	const static DWORD cod4x_entry = (DWORD)GetModuleHandleA("cod4x_021.dll");
	const static HWND* hwnd = reinterpret_cast<HWND*>(game::cod4x_entry + 0x43FE9A0);
	const static char* prefix = COD4QOL_CONSOLE_PREFIX;
	const static char* localization = *reinterpret_cast<const char**>(0xCC147D4);

	static IDirect3DDevice9** dx9_device_ptr = reinterpret_cast<IDirect3DDevice9**>(0xCC9A408);

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

	typedef void(*Text_PaintCenter)();
	inline Text_PaintCenter pText_PaintCenter;

	void hookedText_PaintCenter();

	typedef void(*R_SetViewParmsForScene)();
	inline R_SetViewParmsForScene pR_SetViewParmsForScene;

	void hookedR_SetViewParmsForScene();
	
	typedef void(*R_DrawXmodelSkinnedCached)(int a1, int a2, int a3);
	inline R_DrawXmodelSkinnedCached pR_DrawXmodelSkinnedCached;

	void hookedR_DrawXmodelSkinnedCached(int a1, int a2, int a3);

	typedef void(*R_DrawXModelRigidModelSurf)();
	inline R_DrawXModelRigidModelSurf pR_DrawXModelRigidModelSurf;

	void hookedR_DrawXModelRigidModelSurf();

	typedef void(*R_DrawXModelRigidModelSurf_End)();
	inline R_DrawXModelRigidModelSurf_End pR_DrawXModelRigidModelSurf_End;

	void hookedR_DrawXModelRigidModelSurf_End();

	typedef unsigned int(*CG_StartAmbient)(int a1);
	inline CG_StartAmbient pCG_StartAmbient;

	unsigned int hookedCG_StartAmbient(int a1);

	int	Cmd_Argc();
	const char* Cmd_Argv(int arg);

	dvar_s* Find(const char*);
	cmd_function_s* Cmd_AddCommand(const char* cmdname, void(__cdecl* function)());
	void Cbuf_AddText(const char* text, int localClientNum);
	const char* String_Alloc(const char* string);

	typedef void(*Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
		extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void(*Com_PrintMessage_t)(int, const char*, char);
		extern Com_PrintMessage_t Com_PrintMessage;

	typedef void(*DB_LoadXAssets_t)(XZoneInfo* zoneInfo, unsigned int zone_count, int sync);
		extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef void(*Sys_CreateConsole_t)(int);
		extern Sys_CreateConsole_t Sys_CreateConsole;

	typedef game::dvar_s*(*Dvar_RegisterBool_t)(const char* name, int boolean, game::dvar_flags flags, const char* description);
		extern Dvar_RegisterBool_t Dvar_RegisterBool;

	typedef game::dvar_s*(*Dvar_RegisterEnum_t)(const char* name, const char** strings, int integer, game::dvar_flags flags, const char* description);
		extern Dvar_RegisterEnum_t Dvar_RegisterEnum;

	inline void* Cmd_AddCommand_fnc = (void*)(cod4x_entry + 0x2116C);
	inline game::CmdArgs* cmd_args = reinterpret_cast<game::CmdArgs*>(0x1410B40);
	inline game::gclient_s* g_clients = reinterpret_cast<game::gclient_s*>(0x13255A8);

	inline game::Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = Cmd_ExecuteSingleCommand_t(0x4F9AB0);
	inline game::Com_PrintMessage_t Com_PrintMessage = game::Com_PrintMessage_t(0x4FCA50);
	inline game::DB_LoadXAssets_t DB_LoadXAssets = DB_LoadXAssets_t(0x48A2B0);
	inline game::Sys_CreateConsole_t Sys_CreateConsole = Sys_CreateConsole_t(cod4x_entry + 0x7F503);

	inline game::Dvar_RegisterBool_t Dvar_RegisterBool = Dvar_RegisterBool_t(cod4x_entry + 0x2D8F2);
	inline game::Dvar_RegisterEnum_t Dvar_RegisterEnum = Dvar_RegisterEnum_t(cod4x_entry + 0x2DCAF);

	inline game::dvar_s* fs_game = game::Find("fs_game");
	inline game::dvar_s* fs_homepath = game::Find("fs_homepath");

	inline int* modCount = reinterpret_cast<int*>(0xCAF7554);
	inline int* modIndex = reinterpret_cast<int*>(0xCAF7558);
	inline const char** modName = reinterpret_cast<const char**>(0xCAF7354);
	inline const char** modDesc = reinterpret_cast<const char**>(0xCAF7358);
}
