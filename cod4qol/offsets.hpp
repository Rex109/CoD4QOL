#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

namespace offsets
{
	bool Init(const std::string& crc32);
	DWORD GetOffset(const std::string& id);
	bool ApplyPatch(const std::string& id);
	bool RemovePatch(const std::string& id);
	std::string GetCRC32();
	std::string GetVersion();
	std::string GetSupportedVersions();
}
