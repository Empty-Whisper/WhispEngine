#pragma once
#include "Module.h"

class ModuleGUI :public Module {
public:
	ModuleGUI(Application* app, bool enable_true = true);
	~ModuleGUI();

public:
	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();
private:

	bool MenuItemAbout(bool* p_open);

private:
	bool show_demo_window = false;
	bool show_style_window = false;
	bool show_about_window = false;
	bool show_console_window = false;
	bool show_configuration_window = false;

	bool my_bool = true;

};
