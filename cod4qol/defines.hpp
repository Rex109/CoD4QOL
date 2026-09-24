#pragma once
#include "CommitInfo.hpp"

#include <string>
#include <vector>

#define COD4QOL_NAME "CoD4QOL"
#define COD4QOL_BASE_VERSION "1.13.0"
#ifndef _DEBUG
#define COD4QOL_VERSION COD4QOL_BASE_VERSION " (" GIT_COMMIT_HASH ")"
#else
#define COD4QOL_VERSION COD4QOL_BASE_VERSION "-DEBUG" " (" GIT_COMMIT_HASH ")"
#endif
#define COD4QOL_CONSOLE_PREFIX COD4QOL_NAME " " COD4QOL_VERSION ">"

#define COD4QOL_GITHUB_BASEURL "https://api.github.com/repos/Rex109/CoD4QOL"
#define COD4QOL_GITHUB_STABLE COD4QOL_GITHUB_BASEURL "/releases/latest"
#define COD4QOL_GITHUB_NIGHTLY COD4QOL_GITHUB_BASEURL "/releases/tags/nightly"

#define COD4QOL_FILE_NAME "download/cod4qol.zip"

#define COD4QOL_IWD_CRC32 "fb921b14"

#define COD4QOL_COD4X_MODULE "cod4x_021.dll"

#define COD4QOL_OFFSETS_URL "https://raw.githubusercontent.com/Rex109/CoD4QOL-Patches/main/offsets.json"
#define COD4QOL_OFFSETS_FILE "cod4qol_offsets.json"