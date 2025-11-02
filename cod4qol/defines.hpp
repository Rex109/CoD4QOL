#pragma once
#include "CommitInfo.hpp"

#include <string>
#include <vector>

#define COD4QOL_NAME "CoD4QOL"
#define COD4QOL_BASE_VERSION "1.9.1"
#ifndef _DEBUG
#define COD4QOL_VERSION COD4QOL_BASE_VERSION " (" GIT_COMMIT_HASH ")"
#else
#define COD4QOL_VERSION COD4QOL_BASE_VERSION "-DEBUG" " (" GIT_COMMIT_HASH ")"
#endif
#define COD4QOL_CONSOLE_PREFIX COD4QOL_NAME " " COD4QOL_VERSION ">"

#define COD4QOL_GITHUB_URL "https://api.github.com/repos/Rex109/CoD4QOL/releases/latest"
#define COD4QOL_FILE_NAME "download/cod4qol.zip"

#define COD4QOL_IWD_CRC32 "fb921b14"

#define COD4QOL_SUPPORTEDVERSIONS "21.1, 21.2, 21.3"

#define COD4QOL_COD4X_MODULE "cod4x_021.dll"

#define COD4QOL_COD4X_CRC32_212 "5b855796"
#define COD4QOL_COD4X_CRC32_211 "b1822ed5"
#define COD4QOL_COD4X_CRC32_213 "ee0fba9d"
#define COD4QOL_COD4X_CRC32_213_INSTALLER "91e9995d"

inline const std::vector<std::string> supported_cod4x_crc32 = { COD4QOL_COD4X_CRC32_213_INSTALLER, COD4QOL_COD4X_CRC32_213, COD4QOL_COD4X_CRC32_212, COD4QOL_COD4X_CRC32_211 };