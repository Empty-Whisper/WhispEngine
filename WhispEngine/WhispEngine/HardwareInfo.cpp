#include <string>
#include "mmgr/mmgr.h"
#include "HardwareInfo.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "Globals.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "GPUdetect/DeviceId.h"


HardwareInfo::HardwareInfo()
{
	SDL_GetVersion(&sdl_version);
	s_sdl_version = std::to_string(sdl_version.major) + '.' + std::to_string(sdl_version.minor) + '.' + std::to_string(sdl_version.patch);

	uint vendor_id, device_id;
	Uint64 mp_buget, mb_usage, mb_available, vmb_reserved;
	std::wstring brand;

	if (getGraphicsDeviceInfo(&vendor_id, &device_id, &brand, &mp_buget, &mb_usage, &mb_available, &vmb_reserved))
	{
		hardware.gpu_vendor = vendor_id;
		hardware.gpu_device = device_id;
		sprintf_s(hardware.gpu_brand, 250, "%S", brand.c_str());
		hardware.vram_mb_budget = float(mp_buget) / (1024.f * 1024.f);
		hardware.vram_mb_usage = float(mb_usage) / (1024.f * 1024.f);
		hardware.vram_mb_available = float(mb_available) / (1024.f * 1024.f);
		hardware.vram_mb_reserved = float(vmb_reserved) / (1024.f * 1024.f);
	}

	SetCapsHardware();	
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

bool HardwareInfo::SetVRAMHardware()
{
	bool ret = false;
	Uint64 mb_budget, mb_usage, mb_available, mb_reserved;
	float to_mb = 1024.f * 1024.f;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &mb_budget, &mb_usage, &mb_available, &mb_reserved))
	{
		hardware.vram_mb_budget = float(mb_budget) / to_mb;
		hardware.vram_mb_usage = float(mb_usage) / to_mb;
		hardware.vram_mb_available = float(mb_available) / to_mb;
		hardware.vram_mb_reserved = float(mb_reserved) / to_mb;

		ret = true;
	}

	return ret;
}

void HardwareInfo::UpdateMemory()
{
	sMStats stats = m_getMemoryStatistics();

	config.total_reported_mem = stats.totalReportedMemory;
	config.total_actual_mem = stats.totalActualMemory;
	config.peak_reported_mem = stats.peakReportedMemory;
	config.peak_actual_mem = stats.peakActualMemory;
	config.accumulated_reported_mem = stats.accumulatedReportedMemory;
	config.accumulated_actual_mem = stats.accumulatedActualMemory;
	config.accumulated_alloc_unit = stats.accumulatedAllocUnitCount;
	config.total_alloc_unity_count = stats.totalAllocUnitCount;
	config.peak_alloc_unit_count = stats.peakAllocUnitCount;
}

const char * HardwareInfo::GetCapsHardware()
{
	return s_caps_hardware.data();
}

void HardwareInfo::SetCapsHardware()
{
	s_caps_hardware.clear();

	if (SDL_Has3DNow())
		s_caps_hardware.append("3DNow, ");
	if (SDL_HasAVX())
		s_caps_hardware.append("AVX, ");
	if (SDL_HasAVX2())
		s_caps_hardware.append("AVX2, ");
	if (SDL_HasMMX())
		s_caps_hardware.append("MMX, ");
	if (SDL_HasRDTSC())
		s_caps_hardware.append("RDTSC, ");
	if (SDL_HasSSE())
		s_caps_hardware.append("SSE, ");
	if (SDL_HasSSE2())
		s_caps_hardware.append("SSE2, ");
	if (SDL_HasSSE3())
		s_caps_hardware.append("SSE3, ");
	if (SDL_HasSSE41())
		s_caps_hardware.append("SSE41, ");
	if (SDL_HasSSE42())
		s_caps_hardware.append("SSE42, ");
}
