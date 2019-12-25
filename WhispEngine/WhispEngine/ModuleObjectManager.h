#pragma once
#include "Module.h"
#include "ComponentMesh.h"
#include "Assimp/include/mesh.h"
#include "Imgui/ImGuizmo.h"
#include "Octree.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/AABB.h"

enum class Primitives {
	NONE = -1,
	CUBE, TETRAHEDRON, OCTAHEDRON, DODECAHEDRON, ICOSAHEDRON,
	SPHERE, HEMISPHERE, TORUS, CONE, CYLINDER,
	MAX
};

struct vector3 {
	float x = 0;
	float y = 0;
	float z = 0;
};
struct rot {
	float axis[3]{ 0.f,0.f,0.f };
	float angle = 0.f; //in radians
};
struct Object_data {
	vector3 pos;
	rot rotate;
	vector3 scale{ 1.f,1.f,1.f };

	float face_color[3]{ 1.f,1.f,1.f };
	float wire_color[3]{ 0.f,0.f,0.f };

	int normals[3]{0, 1, 2};

	float radius = 0.1f;
	int slices = 3;
	int rings = 3;
};

class ModuleObjectManager :
	public Module
{
	friend class GameObject;
public:
	ModuleObjectManager();
	~ModuleObjectManager();

	bool Start();
	update_status Update();
	void UpdateGameObject(GameObject* &i);
	bool CleanUp();

	GameObject* CreateGameObject(GameObject* parent);
	void DestroyGameObject(GameObject* obj);
	void ResetObjects();

	GameObject* GetRoot() const;
	void GetChildsFrom(GameObject* &obj, std::vector<GameObject*> &vector) const;
	const AABB GetMaxAABB(GameObject* obj, std::vector<GameObject*> &vector) const;
	GameObject*	GetSelected() const;
	void SetSelected(GameObject* select);

	void ChangeHierarchy(GameObject * dst, GameObject * src);

	void LuaRegister() override;

	void MousePicking(); 

	bool SaveGameObjects(nlohmann::json &file, GameObject* root = nullptr);
	bool LoadGameObjects(const nlohmann::json &file);
	bool LoadScripts(const nlohmann::json &file);
	bool LoadGameObject(const nlohmann::json &node, GameObject* parent);
	bool LoadScript(const nlohmann::json &node);

	void RefreshObjectsUIDMap();
	void RefreshUIDMap(GameObject* obj);

	void UpdateGuizmo();
	void ChangeGuizmoOperation(ImGuizmo::OPERATION &gizmoOperation);
	void ChangeGuizmoMode(ImGuizmo::MODE &gizmoMode);
	void FillMatrix(float4x4 &matrix,float o[]);

	GameObject* Find(const uint64& uid) const;

	float* CalculateFaceNormals(const float* vertex, const uint &n_face_normals, const uint &n_index, const uint* index, float magnitude = 0.5f);
	// =========================================================== old
	//TODO: Try to fix Primitives and old way to create meshes ----------------------------------------------------------------------------------------------------------

	/*Mesh_info* CreateMesh(const uint &n_vertex, const float* vertex, const uint &n_index, const uint* index, const float* normals, const float* texCoords, ComponentMesh* component);
	Mesh_info* CreateMesh(const aiMesh *mesh, ComponentMesh* component);

	Mesh_info* CreateMeshPrimitive(const Primitives &type, ComponentMesh* component);
	bool CreatePrimitive(const Primitives &type, const Object_data &data);


	void   FillNormals(Mesh_info * ret, const float * normals = nullptr);*/
	/*void   FillIndex(Mesh_info * ret, const uint & n_index, const aiFace* faces);
	void   FillIndex(Mesh_info * ret, const uint & n_index, const uint* index);
	void   FillVertex(Mesh_info * ret, const uint & n_vertex, const float* vertex);
	void   FillTextureCoords(Mesh_info* mesh, const float* textureCoords);
	*/
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	/*void FillNormals(Mesh_info * ret, const float * normals = nullptr);
	void FillIndex(Mesh_info * ret, const uint & n_index, const aiFace* faces);
	void FillIndex(Mesh_info * ret, const uint & n_index, const uint* index);
	void FillVertex(Mesh_info * ret, const uint & n_vertex, const float* vertex);
	void FillTextureCoords(Mesh_info* mesh, const float* textureCoords);
 	float* CalculateFaceNormals(const float* vertex, const uint &n_face_normals, const uint &n_index, const uint* index, float magnitude = 0.5f);*/

	//void Demo();

	// =========================================================== !old

private:
	//const char* PrimitivesToString(const Primitives prim);
	std::map<uint64, GameObject*> objects;

public:
	GameObject* root = nullptr;

	std::map<GameObject*, GameObject*> to_change;
private:
	GameObject* selected = nullptr;

	ImGuizmo::OPERATION   gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE        guizmoMode = ImGuizmo::MODE::WORLD;
	math::float4x4		  transform_changed = float4x4::identity;
};

