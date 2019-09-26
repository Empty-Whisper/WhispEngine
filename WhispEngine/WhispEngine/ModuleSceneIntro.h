#pragma once
#include "Module.h"
#include "Imgui/imgui.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	//TEST
	char * a_inter_b = nullptr;
	char * b_inter_c = nullptr;
	char * a_inter_c = nullptr;
	char * count_down = nullptr;
	Timer timer;
	int inum = 0;
	float fnum = 0.0f;
};