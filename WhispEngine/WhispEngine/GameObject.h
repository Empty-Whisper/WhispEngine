#pragma once

#include "Globals.h"
#include "SDL/include/SDL_config.h"

class GameObject
{
public:
	GameObject();
	void InitColors();
	GameObject(const int &n_vertex, float* vertex, const int &n_index, uint* index, float* normals = nullptr);
	
	//GameObject(const par_shape *shape, Vector3¿?);
	~GameObject();

public:

	void SetGLBuffers();

	void Draw();

	void DrawNormals();

public:
	uint id_vertex = 0;
	uint n_vertex = 0;
	float * vertex = nullptr;

	uint id_index = 0;
	uint n_index = 0;
	uint *index = nullptr;

	float *middle_point = nullptr;

	uint id_normals = 0;
	float* normals = nullptr;
	bool vertex_normals = false;

	bool active = true;
	float* color = nullptr;
	float* wire_color = nullptr;
};

