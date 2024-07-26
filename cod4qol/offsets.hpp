#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

namespace offsets
{
	typedef std::unordered_map<std::string, DWORD> offset;

	void InitOffsets();
	void AddOffset(std::string name, offsets::offset offset);
	DWORD GetOffset(std::string name);
	void SetCRC32(std::string crc32);
}