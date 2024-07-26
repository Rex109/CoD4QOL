#include "offsets.hpp"
#include <unordered_map>
#include <string>
#include <cassert>
#include "game.hpp"
#include "defines.hpp"

std::unordered_map<std::string, offsets::offset> offset_map;
std::string current_crc32;

void offsets::InitOffsets()
{
	//Hooks
	AddOffset("safechecks", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x82E40)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x43580)} });
	AddOffset("faststartup", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x30F00)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x3AA7C)} });
	AddOffset("DB_LoadXZoneFromGfxConfig", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x3C180)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x8327E)} });
	AddOffset("CG_Respawn", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x3DF00)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x3B45)} });
	AddOffset("ScreenshotRequest", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0xB21E0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0xEA610)} });

	//General
	AddOffset("hwnd", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x443BA00)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x43FE9A0)} });
	AddOffset("iwd_cheat_flag", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x2E366)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x386E2)} });
	AddOffset("ss_switch", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0xB21FB)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0xEA62B)} });

	//Functions
	AddOffset("Cmd_AddCommand_fnc", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x639B0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2116C)} });
	AddOffset("Sys_CreateConsole", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x7F160)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x7F503)} });
	AddOffset("Cvar_RegisterBool", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60BE0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2D8F2)} });
	AddOffset("Cvar_RegisterEnum", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60640)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2DCAF)} });
	AddOffset("Cvar_RegisterString", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60960)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2D87D)} });
	AddOffset("FS_AddSingleIwdFileForGameDirectory", { {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x2E310)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x3867C)} });
}

void offsets::AddOffset(std::string id, offsets::offset offset)
{
	assert(offset_map.find(id) == offset_map.end());

	offset_map[id] = offset;
}

DWORD offsets::GetOffset(std::string id)
{
	assert(!current_crc32.empty());
	assert(offset_map.find(id) != offset_map.end());

	DWORD offset = offset_map[id][current_crc32];

	std::cout << "Requested offset for " << id << ": " << offset << std::endl;

	return offset;
}

void offsets::SetCRC32(std::string crc32)
{
	assert(std::find(supported_cod4x_crc32.begin(), supported_cod4x_crc32.end(), crc32) != supported_cod4x_crc32.end());

	std::cout << "Setting CRC32 to: " << crc32 << std::endl;

	current_crc32 = crc32;
}