#pragma once
#include <Windows.h>

#include "detours.h"
#pragma comment(lib, "detours.lib")

namespace hooks
{
	void InitializeHooks();
	LONG install(void** ppPointer, void* pDetour);
	void write_addr(DWORD addr, const char* bytes, size_t len);
}