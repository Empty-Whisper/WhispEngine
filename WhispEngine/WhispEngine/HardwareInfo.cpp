#include "HardwareInfo.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "Globals.h"
#include <string>


HardwareInfo::HardwareInfo()
{
	SDL_GetVersion(&sdl_version);
	s_sdl_version = std::to_string(sdl_version.major) + '.' + std::to_string(sdl_version.minor) + '.' + std::to_string(sdl_version.patch);
}


HardwareInfo::~HardwareInfo()
{
}

int HardwareInfo::GetCPUCacheLineSize()
{
	return SDL_GetCPUCacheLineSize();
}

int HardwareInfo::GetCPUCount()
{
	return SDL_GetCPUCount();
}

int HardwareInfo::GetSystemRAM()
{
	return SDL_GetSystemRAM();
}

const char * HardwareInfo::GetSDLVersion()
{
	return s_sdl_version.data();
}
