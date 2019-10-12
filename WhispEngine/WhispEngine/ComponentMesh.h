#pragma once
#include "Component.h"
#include "Globals.h"
#include "SDL/include/SDL_config.h"

class GameObject;

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
};

enum class Normals {
	NONE = 0, FACE, VERTEX, MAX
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject *parent);
	~ComponentMesh();

private:
	void InitColors();

public:
	void SetColors(const float* face_color = nullptr, const float* wire_color = nullptr);

	void Update();

	void Draw();
	void DrawWireFrame();

	void DrawNormals();

public:
	Mesh_info* mesh = nullptr;

	Normals normals_state = Normals::NONE;

	bool active = true;

	float* color = nullptr;
	float* wire_color = nullptr;
};

