#pragma once
#include "Module.h"
#include "imgui/imgui.h"
#include "Panel.h"

class PanelConfiguration;
class PanelAbout;
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

	bool MenuWindowConsole();


private:
	bool show_demo_window = false;
	bool show_style_window = false;
	bool show_console_window = true;

	bool checkbox_mit_license_window = false;

	std::vector<Panel*> panels;

	ImGuiTextBuffer buffer;
	bool log_new_line = false;

	PanelConfiguration* config = nullptr;
	PanelAbout* about = nullptr;
};
