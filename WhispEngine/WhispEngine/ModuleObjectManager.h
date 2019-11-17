#pragma once
#include "Module.h"
#include "ComponentMesh.h"
#include "Assimp/include/mesh.h"

#include "GameObject.h"

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
public:
	ModuleObjectManager();
	~ModuleObjectManager();

	bool Start();
	update_status Update();
	void UpdateGameObject(GameObject* &i);
	bool CleanUp();

	GameObject* CreateGameObject(GameObject* parent);
	void DestroyGameObject(GameObject* obj);

	GameObject* GetRoot() const;
	void GetAllGameObjects(GameObject* &obj, std::vector<GameObject*> &vector);
	GameObject*	GetSelected() const;
	void		SetSelected(GameObject* select);

	std::vector<Texture*>* GetTextures();

	// ========================================== old

	void AddTexture(Texture *tex);

	Mesh_info* CreateMesh(const uint &n_vertex, const float* vertex, const uint &n_index, const uint* index, const float* normals, const float* texCoords);
	Mesh_info* CreateMesh(const aiMesh *mesh);

	Mesh_info* CreateMeshPrimitive(const Primitives &type);
	bool CreatePrimitive(const Primitives &type, const Object_data &data);

	void FillNormals(Mesh_info * ret, const float * normals = nullptr);
	void FillIndex(Mesh_info * ret, const uint & n_index, const aiFace* faces);
	void FillIndex(Mesh_info * ret, const uint & n_index, const uint* index);
	void FillVertex(Mesh_info * ret, const uint & n_vertex, const float* vertex);
	void FillTextureCoords(Mesh_info* mesh, const float* textureCoords);

	void Demo();

	// =========================================================== !old

private:
	const char* PrimitivesToString(const Primitives prim);

public:
	GameObject* root = nullptr; //TODO: Change this to private

private:
	//GameObject* root = nullptr;

	GameObject* selected = nullptr;

	// ==========================================================

	std::vector<Texture*> textures; //TEMPORARY

	// ========================================================
};

