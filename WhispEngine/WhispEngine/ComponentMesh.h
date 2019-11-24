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
class ResourceMesh;

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

	void Draw(const ResourceMesh* mesh);
	void DrawWireFrame(const ResourceMesh* mesh);
	void DrawOutline(const ResourceMesh* mesh);
	void DrawNormals(const ResourceMesh* mesh);

	void OnInspector();

	void SetAABB();
	AABB GetAABB() const;
	void CalulateAABB_OBB();
	OBB  GetOBB() const;

	void Save(nlohmann::json &node) override;
	void Load(const nlohmann::json &node) override;

public:
	uint64 uid = 0u;
	Normals normals_state = Normals::NONE;

private:
	bool active = true;

	bool view_face_normals = false;
	bool view_vertex_normals = false;

	/*Linked to a material to know if it has to use a color or a texture.
	  Check every frame if exist a material in GameObject will consume a lot of resources*/
	ComponentMaterial * material = nullptr;

public:
	AABB local_box;
	AABB aabb;
	OBB obb;
};

