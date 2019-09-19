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
};
