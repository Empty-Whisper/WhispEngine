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
	Buffer<float> normals;
	/*uint id_vertex = 0;
	uint n_vertex = 0;
	float * vertex = nullptr;

	uint id_index = 0;
	uint n_index = 0;
	uint *index = nullptr;*/

	/*float *middle_point = nullptr;

	uint id_normals = 0;
	float* normals = nullptr;*/

	bool active = true;
	float* color = nullptr;
	float* wire_color = nullptr;
};

