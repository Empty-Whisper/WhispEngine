#pragma once
#include "Module.h"
#include "Imgui/imgui.h"

#include "par_shapes.h"
#include "ComponentMesh.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	void DrawGrid();
	bool CleanUp();

	void GenerateGrid(const int &width);

	bool SaveScene();
	bool LoadScene(const char* scene) const;

public:
	uint grid_id = 0;
	uint grid_vertex_size = 0;
	bool show_grid = true;

private:
	std::string scene_name;
};