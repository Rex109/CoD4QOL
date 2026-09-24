#include <Windows.h>
#include <cstdio>
#include "hooks.hpp"
#include "game.hpp"
#include "crc32/crc32.h"
#include <fstream> 
#include "updater.hpp"
#include <thread>
#include "offsets.hpp"
#include "exception.hpp"

void Initialize();
bool GetCoD4XCRC32(std::string& hash);

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

            SetUnhandledExceptionFilter(exception::ExceptionFilter);

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

    std::string crc32;

    if (!GetCoD4XCRC32(crc32) || !offsets::Init(crc32))
    {
        std::string message = "CoD4X version mismatch, CoD4QOL has been unloaded.\nSupported CoD4X versions are: " + offsets::GetSupportedVersions() + ".\n\nCoD4QOL downloads support for new CoD4X versions automatically: make sure you are connected to the internet and restart your game.\n\nIf this keeps happening, support for this CoD4X version hasn't been published yet.";
        MessageBox(NULL, message.c_str(), "CoD4QOL", MB_ICONWARNING);
        return;
    }

    game::SetCoD4xFunctionOffsets();
    hooks::InitializeHooks();
}

bool GetCoD4XCRC32(std::string& hash)
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
    hash = crc32(data, size);
    delete[] data;

    std::cout << "CoD4X CRC32: " << hash << std::endl;
    return true;
}
