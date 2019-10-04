#pragma once

#include "Globals.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

public:
	void Draw();

public:
	uint id_vertex = 0;
	uint n_vertex = 0;
	float * vertex = nullptr;

	uint id_index = 0;
	uint n_index = 0;
	uint *index = nullptr;
};

