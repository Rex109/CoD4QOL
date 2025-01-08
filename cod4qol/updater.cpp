#include "updater.hpp"
#include "defines.hpp"
#include "json.hpp"
#define CURL_STATICLIB
#include "curl/curl.h"
#include "game.hpp"
#include "zip/zip.h"

std::string download_url;

bool updater::CheckForUpdates()
{
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    std::string readBuffer;

    curl_easy_setopt(curl, CURLOPT_URL, COD4QOL_GITHUB_URL);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, updater::WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, COD4QOL_NAME);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (res != CURLE_OK) {
        std::cout << "Failed to check new version: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    nlohmann::json parsed = nlohmann::json::parse(readBuffer);

    if (parsed["tag_name"] == COD4QOL_BASE_VERSION)
    {
        std::cout << "You are using the latest version of " << COD4QOL_NAME << std::endl;
        return false;
    }

    download_url = parsed["assets"][0]["browser_download_url"];

    std::cout << "A new version of " << COD4QOL_NAME << " is available: " << parsed["tag_name"] << std::endl;
    game::Cmd_ExecuteSingleCommand(0, 0, "set qol_dialog 1\n");
    return true;
}

size_t updater::WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void updater::Update()
{
    if(download_url.empty())
        return;

    std::filesystem::create_directory("download");

    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    FILE* fp;
    fopen_s(&fp, COD4QOL_FILE_NAME, "wb");

    if (!fp)
    {
		std::cout << "Failed to open file for writing" << std::endl;
		return;
	}

    curl_easy_setopt(curl, CURLOPT_URL, download_url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, updater::FileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, COD4QOL_NAME);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl); 

    fclose(fp);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    if (res != CURLE_OK) {
        std::cout << "Failed to download new version: " << curl_easy_strerror(res) << std::endl;
        std::filesystem::remove("download");
        return;
    }

    zip_extract(COD4QOL_FILE_NAME, "./download", NULL, NULL);

    char command[1024];
    snprintf(command, sizeof(command), "\"cmd.exe /Q /c \"@echo off & echo Updating CoD4QOL... & taskkill /f /im iw3mp.exe >nul 2>&1 & ping 127.0.0.1 -n 5 > nul & move /y download\\miles\\cod4qol.asi .\\miles & move /y download\\qol.ff .\\zone\\%s \" & rmdir /s /q download & start iw3mp.exe & exit\"", game::localization);
    system(command);
}

size_t updater::FileCallback(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}