#pragma once
#include "Module.h"
#include "Imgui/imgui.h"

#include "par_shapes.h"

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


	int cube_num_vertices= 8;
	int cube_num_index= 36;
	uint vao = 1;          // Vertex array object
	uint index_vbo = 0;    // Index buffer object

	uint shape = 0;
	uint shape2 = 0;
	par_shapes_mesh* m = nullptr;

	int piramid_num_vertices = 54;
	int piramid_num_index = 8;


};