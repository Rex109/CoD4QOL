#include <Windows.h>
#include <cstdio>
#include "hooks.hpp"
#include "game.hpp"
#include "crc32/crc32.h"
#include <fstream> 
#include "updater.hpp"
#include <thread>


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

bool CheckCoD4XVersion()
{
    if (!game::cod4x_entry)
        return false;

    char buffer[MAX_PATH];
    GetModuleFileNameA(reinterpret_cast<HMODULE>(game::cod4x_entry), buffer, MAX_PATH);

    std::ifstream file(buffer, std::ios::binary);
    if (!file.is_open())
        return false;

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* data = new char[size];
    file.read(data, size);
    file.close();

    CRC32 crc32;
    std::string hash = crc32(data, size);
    delete[] data;

    for (std::string supported_hash : supported_cod4x_crc32)
    {
        std::cout << "Comparing CRC32: " << hash << " with " << supported_hash << std::endl;

        if (hash == supported_hash)
        {
            std::cout << "Passed CRC32 check!" << std::endl;
            return true;
        }
    }

    std::cout << "Failed CRC32 check!" << std::endl;
    return false;
}

void Initialize()
{
    const HMODULE iw3mp = GetModuleHandleA("iw3mp.exe");

    if (!iw3mp)
        return;

    if (!CheckCoD4XVersion())
    {
        MessageBox(NULL, "CoD4X version mismatch, CoD4QOL has been unloaded.\nSupported CoD4X versions are: " COD4QOL_SUPPORTEDVERSIONS ".\n\nYou may need to update your game or manually download a newer version of CoD4QOL, otherwise you will have to wait for a new version of CoD4QOL and manually update it later." , "CoD4QOL", MB_ICONWARNING);
        return;
    }

    hooks::InitializeHooks();
}