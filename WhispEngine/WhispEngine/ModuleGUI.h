#pragma once
#include "Module.h"
#include <vector>
#define MAX_FPS_COUNT 120

#define IMGUI_COLOR_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_COLOR_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_COLOR_PINK ImVec4(1.0f,0.0f,1.0f,1.0f)
#define IMGUI_COLOR_YELLOW ImVec4(1.0f,1.0f,0.0f,1.0f)
#define IMGUI_COLOR_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_COLOR_WHITE ImVec4(1.f,1.f,1.f,1.f)



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

	uint gpu_vendor = 0;
	uint gpu_device = 0;
	char gpu_brand[250] = "";
	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;
	float vram_mb_reserved = 0.f;
};

class ModuleGUI :public Module {
public:
	ModuleGUI(bool enable_true = true);
	~ModuleGUI();

public:
	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();

public:
	void Log(const char* str);

private:

	bool MenuWindowAbout();
	bool MenuWindowConsole();
	bool MenuWindowConfiguration();

	bool FillVectorFPS();
	bool FillVectorMS();

	std::string ModuleGUI::FindCapsHardware();
	bool FindVRAMHardware();

private:
	bool show_demo_window = false;
	bool show_style_window = false;
	bool show_about_window = false;
	bool show_mit_license_window = false;
	bool show_console_window = true;
	bool show_configuration_window = false;

	int max_fps = 0;
	std::vector<float> ms_reg;
	std::vector<float> fps_reg;

	Application_config config;
	Gpu_detect hardware;
	SDL_version sdl_version;

	ImGuiTextBuffer buffer;
	bool log_new_line = false;
};
