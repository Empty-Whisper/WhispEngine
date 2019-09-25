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

	bool MenuWindowAbout(bool* p_open);
	bool MenuWindowConsole(bool* p_open);
	bool MenuWindowConfiguration(bool* p_open);

private:
	bool show_demo_window = false;
	bool show_style_window = false;
	bool show_about_window = false;
	bool show_mit_license_window = false;
	bool show_console_window = false;
	bool show_configuration_window = false;

	int actual_fps = 0;
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
