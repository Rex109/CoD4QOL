#pragma once
#include <string>
#include <vector>

#define COD4QOL_NAME "CoD4QOL"
#define COD4QOL_BASE_VERSION "1.6.0"
#ifndef _DEBUG
#define COD4QOL_VERSION COD4QOL_BASE_VERSION
#else
#define COD4QOL_VERSION COD4QOL_BASE_VERSION "-DEBUG"
#endif
#define COD4QOL_CONSOLE_PREFIX COD4QOL_NAME " " COD4QOL_VERSION ">"

#define COD4QOL_GITHUB_URL "https://api.github.com/repos/Rex109/CoD4QOL/releases/latest"
#define COD4QOL_FILE_NAME "download/cod4qol.zip"

#define COD4QOL_SUPPORTEDVERSIONS "21.1, 21.2"

#define COD4QOL_COD4X_MODULE "cod4x_021.dll"
inline std::vector<std::string> supported_cod4x_crc32 = { "5b855796" };