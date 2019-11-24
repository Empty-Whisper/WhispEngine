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

	bool SaveCurrentScene();
	bool SaveTemporaryScene() const;
	bool LoadTemporaryScene();
	bool LoadScene(const char* scene);
	bool CreateEmptyScene(const char* name);

	void DebugOctree();

	std::string GetSceneName() const;

public:
	uint grid_id = 0;
	uint grid_vertex_size = 0;
	bool show_grid = true;
	bool show_octree = false;
	bool show_mouse_raycast = false;

	OctreeTree* octree = nullptr;

	float3 mouse_pick_0 = float3::zero;
	float3 mouse_pick_1 = float3::zero;

	std::string scene_path;
};