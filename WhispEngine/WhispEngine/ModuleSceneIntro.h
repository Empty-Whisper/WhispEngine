#pragma once
#include "Module.h"
#include "Imgui/imgui.h"

#include "par_shapes.h"
#include "GameObject.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	par_shapes_mesh* m = nullptr;

	GameObject *cube = nullptr;

};