#pragma once

#include "Globals.h"
#include "SDL/include/SDL_config.h"

class GameObject
{
public:
	GameObject() {}
	GameObject(const int &n_vertex, const float* vertex, const int &n_index, const uint* index);
	
	//GameObject(const par_shape *shape, Vector3¿?);
	~GameObject();

public:

	void SetGLBuffers();

	void Draw();

public:
	uint id_vertex = 0;
	uint n_vertex = 0;
	float * vertex = nullptr;

	uint id_index = 0;
	uint n_index = 0;
	uint *index = nullptr;
};

