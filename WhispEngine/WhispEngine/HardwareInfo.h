#pragma once
#include "SDL/include/SDL_version.h"
#include <string>

class HardwareInfo
{
public:
	HardwareInfo();
	~HardwareInfo();

	int GetCPUCacheLineSize();
	int GetCPUCount();
	int GetSystemRAM();
	const char* GetSDLVersion();

private:
	SDL_version sdl_version;
	std::string s_sdl_version;
};

