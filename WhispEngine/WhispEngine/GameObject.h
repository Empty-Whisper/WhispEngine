#pragma once

#include "Globals.h"
#include "SDL/include/SDL_config.h"
#include <vector>

template <class T>
struct Buffer {
	uint id = 0;
	uint size = 0;
	T* data = nullptr;
};

struct Mesh {
	~Mesh();

	void SetGLBuffers();

	Buffer<float> vertex;
	Buffer<uint> index;
	Buffer<float> face_normals;
	Buffer<float> vertex_normals;
};

class GameObject
{
public:
	GameObject();
	//GameObject(const int &n_vertex, float* vertex, const int &n_index, uint* index, float* normals = nullptr);

	~GameObject();

private:
	void InitColors();
	void InitNormals();

public:
	void SetColors(const float* face_color = nullptr, const float* wire_color = nullptr);
	void SetNormals(const int* normals = nullptr);

	void Draw();

	void DrawNormals(/*const int* type_normals*/);

public:
	std::vector<Mesh*> mesh;

	bool active = true;
	int* normals = nullptr;
	float* color = nullptr;
	float* wire_color = nullptr;
};

