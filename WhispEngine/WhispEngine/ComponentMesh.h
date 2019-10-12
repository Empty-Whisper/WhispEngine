#pragma once

#include "Globals.h"
#include "SDL/include/SDL_config.h"
#include <vector>

template <typename T>
struct Buffer {
	uint id = 0;
	uint size = 0;
	T* data = nullptr;
};

struct Mesh_info {
	~Mesh_info();

	void SetGLBuffers();

	Buffer<float> vertex;
	Buffer<uint> index;
	Buffer<float> face_normals;
	Buffer<float> vertex_normals;
	Buffer<float> tex_coords;

	uint tex_id = 0;
};

enum class Normals {
	NONE = 0, FACE, VERTEX, MAX
};

class Mesh
{
public:
	Mesh();
	//GameObject(const int &n_vertex, float* vertex, const int &n_index, uint* index, float* normals = nullptr);

	~Mesh();

private:
	void InitColors();

public:
	void SetColors(const float* face_color = nullptr, const float* wire_color = nullptr);

	void Draw();
	void DrawWireFrame();

	void DrawNormals();

public:
	std::vector<Mesh_info*> mesh;

	Normals normals_state = Normals::NONE;

	bool active = true;

	float* color = nullptr;
	float* wire_color = nullptr;
};

