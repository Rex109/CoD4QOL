#include <Windows.h>
#include <cstdio>
#include "hooks.hpp"
#include "game.hpp"

void Initialize();

HMODULE dummy;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);

            #ifdef _DEBUG
                FILE* _con;
                AllocConsole();
                freopen_s(&_con, "CONOUT$", "w", stdout);
            #endif

            //THIS IS NECESSARY! It prevents the library from getting unloaded by cod4
            CHAR FileName[MAX_PATH];
            GetModuleFileName(hModule, FileName, MAX_PATH);
            GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, strrchr(FileName, '\\') + 0x1, &dummy);

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
        return;

    if (!game::cod4x_entry || strcmp(reinterpret_cast<const char*>(game::cod4x_entry + 0x22551A), "CoD4 MP 21.2") != 0)
    {
        MessageBox(NULL, "CoD4X version mismatch, CoD4QOL has been unloaded.\nYou need to have CoD4X version " COD4QOL_SUPPORTEDVERSIONS " installed.\n\nYou may need to update your game or manually download a newer version of CoD4QOL, otherwise you will have to wait for a new version of CoD4QOL and manually update it later." , "CoD4QOL", MB_ICONWARNING);
        return;
    }

    hooks::InitializeHooks();
}