#pragma once

#include "Globals.h"
#include "SDL/include/SDL_config.h"

class GameObject
{
public:
	GameObject(const int &n_vertex, const float* vertex, const int &n_index, const uint16_t* index);
	~GameObject();

public:
	void Draw();

public:
	uint id_vertex = 0;
	uint n_vertex = 0;
	float * vertex = nullptr;

	uint id_index = 0;
	uint n_index = 0;
	uint16_t *index = nullptr;
};

