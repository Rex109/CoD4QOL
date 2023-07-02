#include <Windows.h>
#include <cstdio>
#include "hooks.hpp"

void Initialize();

HMODULE s_hModule;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            s_hModule = hModule;

            #ifdef _DEBUG
                FILE* _con;
                AllocConsole();
                freopen_s(&_con, "CONOUT$", "w", stdout);
            #endif

            Initialize();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

void Initialize()
{
    const HMODULE iw3mp = GetModuleHandleA("iw3mp.exe");

    if (!iw3mp)
    {
        FreeConsole();
        FreeLibraryAndExitThread(s_hModule, 0);
        return;
    }
    
    hooks::InitializeHooks();
}