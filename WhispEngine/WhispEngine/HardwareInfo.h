#pragma once
#include "SDL/include/SDL_version.h"
#include <string>

struct Application_config
{
	int total_reported_mem = 0;
	int total_actual_mem = 0;
	int peak_reported_mem = 0;
	int peak_actual_mem = 0;
	int accumulated_reported_mem = 0;
	int accumulated_actual_mem = 0;
	int accumulated_alloc_unit = 0;
	int total_alloc_unity_count = 0;
	int peak_alloc_unit_count = 0;
};

struct Gpu_detect {

	unsigned int gpu_vendor = 0;
	unsigned int gpu_device = 0;
	char gpu_brand[250] = "";
	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;
	float vram_mb_reserved = 0.f;
};

class HardwareInfo
{
public:
	HardwareInfo();
	~HardwareInfo();

	int GetCPUCacheLineSize();
	int GetCPUCount();
	int GetSystemRAM();

	const char* GetSDLVersion();
	const char* GetOpenGLVersion();
	const char* GetGraphicsCardVendor();
	const char* GetGraphicsCardModel();

	int GetDedicatedMemory();
	int GetTotalAvailableMemory();
	int GetCurrentMemory();
	int GetTotalMemoryEvicted();

	bool SetVRAMHardware();
	void UpdateMemory();

	const char* GetCapsHardware();
	void SetCapsHardware();

public:
	Application_config config;
	Gpu_detect hardware;

private:
	SDL_version sdl_version;
	std::string s_sdl_version;

	std::string s_caps_hardware;
};

