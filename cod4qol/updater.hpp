#pragma once
#include "commands.hpp"
#include "json.hpp"

namespace updater
{
	bool GetString(const nlohmann::json& obj, const char* key, std::string& out);
	bool CheckForUpdates(const char* update_url);
	size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);
	void Update();
	size_t FileCallback(void* ptr, size_t size, size_t nmemb, void* stream);
}