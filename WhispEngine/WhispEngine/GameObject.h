#pragma once

#include "Globals.h"
#include "SDL/include/SDL_config.h"

template <class T>
struct Buffer {
	uint id = 0;
	uint size = 0;
	T* data = nullptr;
};

class GameObject
{
public:
	GameObject();
	//GameObject(const int &n_vertex, float* vertex, const int &n_index, uint* index, float* normals = nullptr);

	~GameObject();

private:
	void InitColors();

public:

	void SetGLBuffers();
	void SetColors(const float* face_color = nullptr, const float* wire_color = nullptr);

	void Draw();

	void DrawNormals();

public:
	Buffer<float> vertex;
	Buffer<uint> index;
	Buffer<float> face_normals;
	Buffer<float> vertex_normals;

	bool active = true;
	float* color = nullptr;
	float* wire_color = nullptr;
};

