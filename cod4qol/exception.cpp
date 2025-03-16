#include "exception.hpp"
#include <DbgHelp.h>
#include <stdio.h>
#include <sstream>
#include "resource.h"

#pragma warning(disable:4996) 
#include <sphelper.h>
#include "game.hpp"
#pragma warning(default: 4996)

#define ID_BUTTON_COPY 1
#define ID_BUTTON_DUMP 2
#define ID_BUTTON_EXIT 3
#define ID_EDIT_CRASH  4
#define ID_STATIC_IMAGE 5

HBITMAP hBitmap;
std::string g_crashDetails;
EXCEPTION_POINTERS* exceptionInfo;

void exception::OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rect;
    GetClientRect(hwnd, &rect);

    int imageWidth = 100;
    int imageHeight = 100;
    int imageX = rect.right - imageWidth - 10; 
    int imageY = 10;

    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdc, imageX, imageY, imageWidth, imageHeight, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
    EndPaint(hwnd, &ps);
}


std::string exception::GetCrashDetails(EXCEPTION_POINTERS* pExceptionInfo)
{
    std::stringstream ss;
    ss << "Exception Code: 0x" << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << "\r\n";
    ss << "Exception Address: 0x" << pExceptionInfo->ExceptionRecord->ExceptionAddress << "\r\n";

    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)pExceptionInfo->ExceptionRecord->ExceptionAddress, &hModule);
    char szModule[MAX_PATH];
    GetModuleFileName(hModule, szModule, MAX_PATH);
    std::string moduleName = szModule;
    moduleName = moduleName.substr(moduleName.find_last_of("\\") + 1);
    ss << std::dec << "Exception Module: " << moduleName << "\r\n\r\n";

    // CoD4QOL version
    ss << "CoD4QOL Version: " COD4QOL_VERSION "\r\n";

    // CPU info
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    ss << "CPU: " << sysInfo.dwNumberOfProcessors << " cores, " << sysInfo.dwPageSize / 1024 << " KB page size\r\n";

    // Memory info
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    ss << "Memory: " << memInfo.ullTotalPhys / 1024 / 1024 << " MB total, " << memInfo.ullAvailPhys / 1024 / 1024 << " MB available\r\n";

    // GPU info
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);
    EnumDisplayDevices(NULL, 0, &dd, 0);
    ss << "GPU: " << dd.DeviceString;

    return ss.str();
}

LRESULT CALLBACK exception::CrashWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", g_crashDetails.c_str(),
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
            10, 10, 460, 200, hwnd, (HMENU)ID_EDIT_CRASH, GetModuleHandle(NULL), NULL);

        CreateWindow("BUTTON", "Copy Details", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 220, 100, 30, hwnd, (HMENU)ID_BUTTON_COPY, GetModuleHandle(NULL), NULL);

        CreateWindow("BUTTON", "Create Dump", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			140, 220, 100, 30, hwnd, (HMENU)ID_BUTTON_DUMP, GetModuleHandle(NULL), NULL);

        CreateWindow("BUTTON", "Exit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            370, 220, 100, 30, hwnd, (HMENU)ID_BUTTON_EXIT, GetModuleHandle(NULL), NULL);

        hBitmap = LoadBitmap(game::GetCurrentModule(), MAKEINTRESOURCE(COD4QOL_CRASH_LOGO));
    }
    break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_BUTTON_COPY:
            {
            OpenClipboard(hwnd);
            EmptyClipboard();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, g_crashDetails.size() + 1);
            if (hMem)
            {
                memcpy(GlobalLock(hMem), g_crashDetails.c_str(), g_crashDetails.size() + 1);
                GlobalUnlock(hMem);
                SetClipboardData(CF_TEXT, hMem);
            }
            CloseClipboard();
            MessageBox(hwnd, "Crash details copied to clipboard!", "CoD4QOL", MB_OK | MB_ICONINFORMATION);
            break;
            }
        case ID_BUTTON_EXIT:
            DeleteObject(hBitmap);
            PostQuitMessage(0);
            break;

        case ID_BUTTON_DUMP:
            CreateCrashDump();
            MessageBox(hwnd, "Crash dump created!", "CoD4QOL", MB_OK | MB_ICONINFORMATION);
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_PAINT:
        OnPaint(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void exception::ShowCrashReportWindow()
{
    const char CLASS_NAME[] = "CrashWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = CrashWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = 600;
    int windowHeight = 300;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Call of Duty 4 - CoD4QOL", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, windowX, windowY, windowWidth, windowHeight, NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void exception::GenerateDumpFileName(TCHAR* filename, size_t size)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    sprintf_s(filename, size, "iw3mp-%02d-%02d-%04d-%02d-%02d-%02d.dmp",st.wDay, st.wMonth, st.wYear,st.wHour, st.wMinute, st.wSecond);
}

void exception::CreateCrashDump()
{
    TCHAR dumpFileName[MAX_PATH];
    GenerateDumpFileName(dumpFileName, MAX_PATH);

    HANDLE hFile = CreateFile(dumpFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = exceptionInfo;
        mdei.ClientPointers = FALSE;

        const auto type = MiniDumpIgnoreInaccessibleMemory
            | MiniDumpWithHandleData
            | MiniDumpScanMemory
            | MiniDumpWithProcessThreadData
            | MiniDumpWithFullMemoryInfo
            | MiniDumpWithThreadInfo
            | MiniDumpWithUnloadedModules;

        if (!MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, static_cast<MINIDUMP_TYPE>(type), &mdei, nullptr, nullptr))
        {
            MessageBox(NULL, "There was an error creating the minidump! Hit OK to close the program.", "CoD4QOL", MB_ICONERROR);
            TerminateProcess(GetCurrentProcess(), 123);
        }

        CloseHandle(hFile);
    }
}

LONG WINAPI exception::ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
{
    g_crashDetails = GetCrashDetails(pExceptionInfo);
    exceptionInfo = pExceptionInfo;

    ShowCrashReportWindow();

    return EXCEPTION_EXECUTE_HANDLER;
}