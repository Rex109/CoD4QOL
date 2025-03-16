#pragma once
#include <Windows.h>
#include <string>

namespace exception
{
	void OnPaint(HWND hwnd);
	void ShowCrashReportWindow();
	void GenerateDumpFileName(TCHAR* filename, size_t size);
	void CreateCrashDump();

	std::string GetCrashDetails(EXCEPTION_POINTERS* pExceptionInfo);
	LRESULT CALLBACK CrashWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo);
}