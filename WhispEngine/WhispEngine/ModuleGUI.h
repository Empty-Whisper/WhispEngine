#pragma once
#include "Module.h"
#include "imgui/imgui.h"
#include <vector>
#define MAX_FPS_COUNT 120

#define IMGUI_COLOR_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_COLOR_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_COLOR_PINK ImVec4(1.0f,0.0f,1.0f,1.0f)
#define IMGUI_COLOR_YELLOW ImVec4(1.0f,1.0f,0.0f,1.0f)
#define IMGUI_COLOR_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_COLOR_WHITE ImVec4(1.f,1.f,1.f,1.f)

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

	void PushBackVectorAsQueue(std::vector<float> &vector, const float &value);

private:
	bool show_demo_window = false;
	bool show_style_window = false;
	bool show_about_window = false;
	bool show_console_window = true;
	bool show_configuration_window = true;

	bool checkbox_mit_license_window = false;
	bool checkbox_fullscreen_window = false;
	bool checkbox_borderless_window = false;
	bool checkbox_fulldesktop_window = false;
	bool checkbox_resizable_window = false;

	int window_width = 0;
	int window_height = 0;

	char app_name[30] = TITLE;
	char organization[30] = ORGANIZATION;

	int max_fps = 0;
	std::vector<float> ms_reg;
	std::vector<float> fps_reg;
	std::vector<float> mem_reg;

	float bright = 1.0f;

	ImGuiTextBuffer buffer;
	bool log_new_line = false;
};
