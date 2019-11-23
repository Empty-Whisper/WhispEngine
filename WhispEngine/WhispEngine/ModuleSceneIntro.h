#pragma once
#include "Module.h"
#include "Imgui/imgui.h"

#include "par_shapes.h"
#include "ComponentMesh.h"
#include "Octree.h"

class GameObject;
class Component;

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
	bool CreateEmptyScene(const char* name);

	void DebugOctree();

	std::string GetSceneName() const;

public:
	uint grid_id = 0;
	uint grid_vertex_size = 0;
	bool show_grid = true;
	bool show_octree = false;

	OctreeTree* octree = nullptr;

	std::string scene_path;
};