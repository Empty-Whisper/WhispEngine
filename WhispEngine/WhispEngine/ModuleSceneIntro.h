#pragma once
#include "Module.h"
#include "Imgui/imgui.h"

#include "par_shapes.h"
#define PAR_PI (3.14159265359)


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


	
	uint vao = 0;          // Vertex array object
	uint index_vbo = 0;    // Index buffer object

	uint id_vertex = 0;
	uint id_index = 0;
	par_shapes_mesh* m = nullptr;

	

};