#pragma once
#include "Module.h"
#include "imgui/imgui.h"
#include "Panel.h"

class PanelConsole;
class PanelConfiguration;
class PanelAbout;
class PanelRender;
class PanelHierarchy;
class PanelCreate;

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
	bool Init(nlohmann::json &node);

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();

	update_status MainMenuBar();

public:
	void Log(const char* str);

private:
	bool show_demo_window = false;
	bool show_style_window = false;
	bool show_demo_create = false;

	std::vector<Panel*> panels;

	ImGuiTextBuffer buffer;
	bool log_new_line = false;

	PanelConfiguration* config = nullptr;
	PanelAbout* about = nullptr;
	PanelConsole* console = nullptr;
	PanelRender* render = nullptr;
	PanelHierarchy* inspector = nullptr;
	PanelCreate* create = nullptr;
};
