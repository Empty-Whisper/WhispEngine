#include "HardwareInfo.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "Globals.h"
#include "glew/glew.h"
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

const char * HardwareInfo::GetOpenGLVersion()
{
	return (const char*)glGetString(GL_VERSION);
}

const char * HardwareInfo::GetGraphicsCardVendor()
{
	return (const char*)glGetString(GL_VENDOR);
}

const char * HardwareInfo::GetGraphicsCardModel()
{
	return (const char*)glGetString(GL_RENDERER);
}

int HardwareInfo::GetDedicatedMemory()
{
	int ret = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &ret);
	return ret/1024;
}

int HardwareInfo::GetTotalAvailableMemory()
{
	int ret = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ret);
	return ret/1024;
}

int HardwareInfo::GetCurrentMemory()
{
	int ret = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ret);
	return ret/1024;
}

int HardwareInfo::GetTotalMemoryEvicted()
{
	int ret = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &ret);
	return ret/1024;
}
