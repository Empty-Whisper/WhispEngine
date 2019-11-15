#pragma once
#include "Component.h"
#include "ComponentMaterial.h"
#include "Globals.h"
#include "SDL/include/SDL_config.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

#include "glmath.h"
#include <array>


class GameObject;
class ComponentMesh;

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

	AABB local_box;
	AABB aabb;
	OBB obb;

	ComponentMesh* component = nullptr;
};

enum class Normals {
	NONE = 0, FACE, VERTEX, MAX
};


class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject *parent);
	~ComponentMesh();

public:

	void Update();

	void Draw();
	void DrawWireFrame();
	void DrawOutline();
	void DrawNormals();

	void OnInspector();
	math::float3 CalculateRadius();

public:
	Mesh_info* mesh = nullptr;
	Normals normals_state = Normals::NONE;


private:
	bool active = true;

	bool view_face_normals = false;
	bool view_vertex_normals = false;

	/*Linked to a material to know if it has to use a color or a texture.
	  Check every frame if exist a material in GameObject will consume a lot of resources*/
	ComponentMaterial * material = nullptr;
};

