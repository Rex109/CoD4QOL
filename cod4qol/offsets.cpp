#include "offsets.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include "game.hpp"
#include "hooks.hpp"
#include "defines.hpp"
#include "resource.h"
#include "json.hpp"
#include "updater.hpp"
#define CURL_STATICLIB
#include "curl/curl.h"

typedef struct
{
	DWORD address;
	std::string bytes;
	std::string original;
}patch_write_t;

typedef struct
{
	std::vector<patch_write_t> writes;
	bool applied;
}patch_t;

//Every offset this build of CoD4QOL needs, a CoD4X version missing any of these counts as unsupported.
//Offsets that are only needed on some versions (like mousefix) are left out and simply not patched when null.
const std::vector<std::string> required_offsets = {
	"safechecks", "faststartup", "DB_LoadXZoneFromGfxConfig", "CG_Respawn", "ScreenshotRequest", "menufps",
	"hwnd", "iwd_flag_localized", "iwd_flag_lang", "iwd_restriction", "ss_switch", "steam_auth_a", "steam_auth_b",
	"BG_WeaponNames", "Cmd_AddCommand_fnc", "Sys_CreateConsole", "Cvar_RegisterBool", "Cvar_RegisterEnum",
	"Cvar_RegisterString", "Cvar_RegisterFloat", "Cvar_RegisterVec4", "Cvar_RegisterInt",
	"FS_AddSingleIwdFileForGameDirectory", "DB_BuildOSPath", "CG_DrawUpperRightDebugInfo"
};

std::unordered_map<std::string, DWORD> offset_map;
std::unordered_map<std::string, patch_t> patch_map;
std::vector<std::string> supported_versions;

std::string current_crc32;
std::string current_version;

static std::string GetLocalPath()
{
	std::filesystem::path dir;

	if (game::fs_savepath && game::fs_savepath->current.string && *game::fs_savepath->current.string)
		dir = game::fs_savepath->current.string;
	else
	{
		//Shouldn't happen, but never lose the file: fall back to the folder cod4qol.asi is in
		char path[MAX_PATH];
		GetModuleFileNameA(game::GetCurrentModule(), path, MAX_PATH);
		dir = std::filesystem::path(path).parent_path();
		std::cout << "fs_savepath is not available, using " << dir.string() << std::endl;
	}

	std::error_code ec;
	std::filesystem::create_directories(dir, ec);

	return (dir / COD4QOL_OFFSETS_FILE).string();
}

static bool ReadTextFile(const std::string& path, std::string& out)
{
	std::ifstream file(path, std::ios::binary);

	if (!file.is_open())
		return false;

	std::stringstream buffer;
	buffer << file.rdbuf();
	out = buffer.str();

	return true;
}

static void WriteTextFile(const std::string& path, const std::string& text)
{
	std::ofstream file(path, std::ios::binary | std::ios::trunc);

	if (!file.is_open())
	{
		std::cout << "Failed to write " << path << std::endl;
		return;
	}

	file.write(text.data(), text.size());
}

static bool LoadEmbedded(std::string& out)
{
	HMODULE module = game::GetCurrentModule();
	HRSRC hRes = FindResource(module, MAKEINTRESOURCE(COD4QOL_OFFSETS), RT_RCDATA);

	if (!hRes)
		return false;

	HGLOBAL hData = LoadResource(module, hRes);

	if (!hData)
		return false;

	out.assign(reinterpret_cast<const char*>(LockResource(hData)), SizeofResource(module, hRes));

	return true;
}

//This runs inside DllMain, while Windows holds the loader lock. libcurl resolves hostnames on a separate thread,
//which can't start until the lock is released, so the timeout keeps a stuck download from freezing the game.
static bool Download(const char* url, std::string& out)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, updater::WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, COD4QOL_NAME);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NO_REVOKE);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

	ULONGLONG start = GetTickCount64();
	CURLcode res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	if (res != CURLE_OK)
	{
		std::cout << "Failed to download offsets after " << GetTickCount64() - start << "ms: " << curl_easy_strerror(res) << std::endl;
		return false;
	}

	std::cout << "Downloaded offsets in " << GetTickCount64() - start << "ms" << std::endl;

	return true;
}

static DWORD GetCoD4XImageSize()
{
	const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(game::cod4x_entry);
	const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(game::cod4x_entry + dos->e_lfanew);

	return nt->OptionalHeader.SizeOfImage;
}

//Offsets are relative to cod4x_021.dll, anything outside of it is rejected
static bool ParseRVA(const nlohmann::ordered_json& value, DWORD size, DWORD& rva)
{
	if (!value.is_string())
		return false;

	try
	{
		rva = std::stoul(value.get<std::string>(), nullptr, 16);
	}
	catch (...)
	{
		return false;
	}

	return rva != 0 && rva + size <= GetCoD4XImageSize();
}

static bool ParseBytes(const nlohmann::ordered_json& value, std::string& out)
{
	if (!value.is_string())
		return false;

	std::istringstream stream(value.get<std::string>());
	std::string byte;

	while (stream >> byte)
	{
		if (byte.size() != 2 || !isxdigit(static_cast<unsigned char>(byte[0])) || !isxdigit(static_cast<unsigned char>(byte[1])))
			return false;

		out.push_back(static_cast<char>(std::stoul(byte, nullptr, 16)));
	}

	return !out.empty();
}

//Loads offsets and patches for crc32 from an offsets.json, only replaces the current ones if everything is valid
static bool LoadFromJson(const std::string& text, const std::string& crc32, const char* source)
{
	nlohmann::ordered_json root = nlohmann::ordered_json::parse(text, nullptr, false);

	if (root.is_discarded() || !root.contains("versions") || !root["versions"].is_object())
	{
		std::cout << "Offsets from " << source << " are malformed" << std::endl;
		return false;
	}

	for (const auto& entry : root["versions"])
	{
		if (!entry.contains("version") || !entry["version"].is_string())
			continue;

		std::string name = entry["version"].get<std::string>();

		if (std::find(supported_versions.begin(), supported_versions.end(), name) == supported_versions.end())
			supported_versions.push_back(name);
	}

	auto version = root["versions"].find(crc32);

	if (version == root["versions"].end() || !version->is_object() || !version->contains("offsets") || !(*version)["offsets"].is_object())
	{
		std::cout << "Offsets from " << source << " don't support this CoD4X version" << std::endl;
		return false;
	}

	std::unordered_map<std::string, DWORD> new_offsets;
	std::unordered_map<std::string, patch_t> new_patches;

	for (const auto& [id, value] : (*version)["offsets"].items())
	{
		//Not needed on this CoD4X version
		if (value.is_null())
			continue;

		DWORD rva;

		if (!ParseRVA(value, 0, rva))
		{
			std::cout << "Offsets from " << source << " have an invalid value for " << id << std::endl;
			return false;
		}

		new_offsets[id] = game::cod4x_entry + rva;
	}

	for (const std::string& id : required_offsets)
	{
		if (new_offsets.find(id) == new_offsets.end())
		{
			std::cout << "Offsets from " << source << " are missing " << id << std::endl;
			return false;
		}
	}

	if (version->contains("patches") && (*version)["patches"].is_object())
	{
		for (const auto& [id, writes] : (*version)["patches"].items())
		{
			if (!writes.is_array())
				return false;

			patch_t patch = {};

			for (const auto& write : writes)
			{
				patch_write_t patch_write = {};
				DWORD rva;

				if (!write.is_object() || !write.contains("bytes") || !write.contains("address") || !ParseBytes(write["bytes"], patch_write.bytes) || !ParseRVA(write["address"], patch_write.bytes.size(), rva))
				{
					std::cout << "Offsets from " << source << " have an invalid patch for " << id << std::endl;
					return false;
				}

				patch_write.address = game::cod4x_entry + rva;
				patch.writes.push_back(patch_write);
			}

			new_patches[id] = patch;
		}
	}

	offset_map = new_offsets;
	patch_map = new_patches;
	current_version = (*version).value("version", "unknown");

	std::cout << "Loaded offsets for CoD4X " << current_version << " from " << source << std::endl;

	return true;
}

bool offsets::Init(const std::string& crc32)
{
	std::cout << "Setting CRC32 to: " << crc32 << std::endl;

	current_crc32 = crc32;

	const std::string local_path = GetLocalPath();
	std::string text;

	//First run or deleted file: start from the copy bundled at build time
	if (!std::filesystem::exists(local_path) && LoadEmbedded(text))
		WriteTextFile(local_path, text);

	if (ReadTextFile(local_path, text) && LoadFromJson(text, crc32, "local file"))
		return true;

	//The bundled copy can be newer than the local one, e.g. after updating CoD4QOL while offline
	if (LoadEmbedded(text) && LoadFromJson(text, crc32, "bundled file"))
	{
		WriteTextFile(local_path, text);
		return true;
	}

	std::cout << "Downloading latest offsets..." << std::endl;

	text.clear();

	if (!Download(COD4QOL_OFFSETS_URL, text) || !LoadFromJson(text, crc32, "download"))
		return false;

	WriteTextFile(local_path, text);

	return true;
}

DWORD offsets::GetOffset(const std::string& id)
{
	auto it = offset_map.find(id);

	if (it == offset_map.end())
	{
		std::cout << "Offset " << id << " is not available on this CoD4X version" << std::endl;
		return 0;
	}

	std::cout << "Requested offset for " << id << ": " << it->second << std::endl;

	return it->second;
}

bool offsets::ApplyPatch(const std::string& id)
{
	auto it = patch_map.find(id);

	if (it == patch_map.end())
	{
		std::cout << "Patch " << id << " is not needed on this CoD4X version" << std::endl;
		return false;
	}

	patch_t& patch = it->second;

	for (patch_write_t& write : patch.writes)
	{
		if (!patch.applied)
			write.original.assign(reinterpret_cast<const char*>(write.address), write.bytes.size());

		hooks::write_addr(write.address, write.bytes.data(), write.bytes.size());
	}

	patch.applied = true;

	return true;
}

bool offsets::RemovePatch(const std::string& id)
{
	auto it = patch_map.find(id);

	if (it == patch_map.end() || !it->second.applied)
		return false;

	patch_t& patch = it->second;

	//Restore in reverse order so overlapping writes come back correctly
	for (auto write = patch.writes.rbegin(); write != patch.writes.rend(); ++write)
		hooks::write_addr(write->address, write->original.data(), write->original.size());

	patch.applied = false;

	return true;
}

std::string offsets::GetCRC32()
{
	return current_crc32;
}

std::string offsets::GetVersion()
{
	return current_version;
}

std::string offsets::GetSupportedVersions()
{
	if (supported_versions.empty())
		return "unknown";

	//offsets.json lists the newest version first
	std::string result;

	for (auto it = supported_versions.rbegin(); it != supported_versions.rend(); ++it)
		result += (result.empty() ? "" : ", ") + *it;

	return result;
}
