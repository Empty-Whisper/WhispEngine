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
	friend class ComponentMesh;

	Mesh_info(ComponentMesh* mesh);
	~Mesh_info();

	void SetGLBuffers();

	Buffer<float> vertex;
	Buffer<uint> index;
	Buffer<float> face_normals;
	Buffer<float> vertex_normals;
	Buffer<float> tex_coords;

	uint64_t uid = 0u;

	ComponentMesh* component = nullptr;
};

enum class Normals {
	NONE = 0, FACE, VERTEX, MAX
};


class ComponentMesh : public Component
{
	friend struct Mesh_info;
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

	AABB GetAABB() const;
	void CalulateAABB_OBB();
	OBB  GetOBB() const;

	void Save(nlohmann::json &node) override;
	void Load(const nlohmann::json &node) override;

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

	AABB local_box;
	AABB aabb;
	OBB obb;
};

