#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

namespace offsets
{
	typedef std::unordered_map<std::string, DWORD> offset_set;

	typedef struct
	{
		std::string data;
		size_t size;
	}data_t;

	typedef std::unordered_map<std::string, data_t> data_set;

	void InitOffsets();
	void AddOffset(std::string id, offset_set offset);
	DWORD GetOffset(std::string id);
	void AddData(std::string id, data_set data);
	data_t GetData(std::string id);
	void SetCRC32(std::string crc32);
	std::string GetCRC32();
}