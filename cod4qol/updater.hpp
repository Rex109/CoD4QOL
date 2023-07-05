#pragma once

namespace updater
{
	bool CheckForUpdates();
	size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);
	void Update();
	size_t FileCallback(void* ptr, size_t size, size_t nmemb, void* stream);
}