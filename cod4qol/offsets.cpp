#include "offsets.hpp"
#include <unordered_map>
#include <string>
#include <cassert>
#include "game.hpp"
#include "defines.hpp"

std::unordered_map<std::string, offsets::offset_set> offset_map;
std::unordered_map<std::string, offsets::data_set> data_map;

std::string current_crc32;

void offsets::InitOffsets()
{
	//Hooks
	AddOffset("safechecks", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x7AA10)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x78AC0)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x82E40)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x43580)} });
	AddOffset("faststartup", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x30700)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x2EAB0)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x30F00)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x3AA7C)} });
	AddOffset("DB_LoadXZoneFromGfxConfig", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x35560)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x33920)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x3C180)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x8327E)} });
	AddOffset("CG_Respawn", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x372E0)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x35670)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x3DF00)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x3B45)} });
	AddOffset("ScreenshotRequest", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0xA9DB0)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0xA7B50)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0xB21E0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0xEA610)} });
	AddOffset("mousefix", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x7651A)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x744AA)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x7E14A)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x801EC)} });

	//General
	AddOffset("hwnd", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x4410880)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x4342880)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x443BA00)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x43FE9A0)} });
	AddOffset("iwd_flag_localized", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x2DB66)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x2C016)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x2E366)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x386E2)} });
	AddOffset("iwd_flag_lang", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x2DB6D)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x2C01D)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x2E36D)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x386EC)} });
	AddOffset("ss_switch", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0xA9DCB)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0xA7B6B)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0xB21FB)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0xEA62B)} });
	AddOffset("steam_auth_a", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x101F2)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x10282)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x10982)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x1A70A)} });
	AddOffset("steam_auth_b", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x101FB)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x1028B)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x1098B)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x1A717)} });
	AddOffset("BG_WeaponNames", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x3E3A40)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x315A40)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x408A40)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x443DDE0)} });

	//Functions
	AddOffset("Cmd_AddCommand_fnc", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x5BDD0)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x5A230)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x639B0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2116C)} });
	AddOffset("Sys_CreateConsole", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x77530)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x75510)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x7F160)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x7F503)} });
	AddOffset("Cvar_RegisterBool", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x59000)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x571E0)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60BE0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2D8F2)} });
	AddOffset("Cvar_RegisterEnum", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x58A60)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x56EB0)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60640)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2DCAF)} });
	AddOffset("Cvar_RegisterString", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x58D80)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x57460)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60960)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2D87D)} });
	AddOffset("Cvar_RegisterFloat", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x59280)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x57970)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x60E60)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2D9DA)} });
	AddOffset("Cvar_RegisterVec4", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x59CF0)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x58190)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x618D0)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2DB45)} });
	AddOffset("Cvar_RegisterInt", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x59530)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x576E0)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x61110)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x2D967)} });
	AddOffset("FS_AddSingleIwdFileForGameDirectory", { {COD4QOL_COD4X_CRC32_213_INSTALLER, (game::cod4x_entry + 0x2DB10)}, {COD4QOL_COD4X_CRC32_213, (game::cod4x_entry + 0x2BFC0)}, {COD4QOL_COD4X_CRC32_212, (game::cod4x_entry + 0x2E310)}, {COD4QOL_COD4X_CRC32_211, (game::cod4x_entry + 0x3867C)} });

	//Strings
	AddData("steam_auth_a", { {COD4QOL_COD4X_CRC32_213_INSTALLER, {"\x90\x90", 2}}, {COD4QOL_COD4X_CRC32_213, {"\x90\x90", 2}}, {COD4QOL_COD4X_CRC32_212, {"\x90\x90", 2}}, {COD4QOL_COD4X_CRC32_211, {"\x90\x90\x90\x90\x90\x90", 6}} });
	AddData("steam_auth_b", { {COD4QOL_COD4X_CRC32_213_INSTALLER, {"\xE9\xC0\x01\x00\x00\x90", 6}}, {COD4QOL_COD4X_CRC32_213, {"\xE9\xC0\x01\x00\x00\x90", 6}}, {COD4QOL_COD4X_CRC32_212, {"\xE9\xC0\x01\x00\x00\x90", 6}}, {COD4QOL_COD4X_CRC32_211, {"\x90\x90\x90\x90\x90\x90", 6}} });
	AddData("steam_auth_a_original", { {COD4QOL_COD4X_CRC32_213_INSTALLER, {"\x75\x0D", 2}}, {COD4QOL_COD4X_CRC32_213, {"\x75\x0D", 2}}, {COD4QOL_COD4X_CRC32_212, {"\x75\x0D", 2}}, {COD4QOL_COD4X_CRC32_211, {"\x0F\x85\xDB\x00\x00\x00", 6}} });
	AddData("steam_auth_b_original", { {COD4QOL_COD4X_CRC32_213_INSTALLER, {"\x0F\x84\xBF\x01\x00\x00", 6}}, {COD4QOL_COD4X_CRC32_213, {"\x0F\x84\xBF\x01\x00\x00", 6}}, {COD4QOL_COD4X_CRC32_212, {"\x0F\x84\xBF\x01\x00\x00", 6}}, {COD4QOL_COD4X_CRC32_211, {"\x0F\x85\xCE\x00\x00\x00", 6}} });
}

void offsets::AddOffset(std::string id, offsets::offset_set offset)
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

void offsets::AddData(std::string id, offsets::data_set data)
{
	assert(data_map.find(id) == data_map.end());

	data_map[id] = data;
}

offsets::data_t offsets::GetData(std::string id)
{
	assert(data_map.find(id) != data_map.end());

	offsets::data_t data = data_map[id][current_crc32];

	std::cout << "Requested data for " << id << std::endl;

	return data;
}

void offsets::SetCRC32(std::string crc32)
{
	assert(std::find(supported_cod4x_crc32.begin(), supported_cod4x_crc32.end(), crc32) != supported_cod4x_crc32.end());

	std::cout << "Setting CRC32 to: " << crc32 << std::endl;

	current_crc32 = crc32;
}

std::string offsets::GetCRC32()
{
	return current_crc32;
}