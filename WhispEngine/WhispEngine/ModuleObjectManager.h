#pragma once
#include "Module.h"
#include "ComponentMesh.h"
#include "Assimp/include/mesh.h"

#include "Texture.h"

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

	// ========================================== old

	void AddObject(ComponentMesh* obj);
	void AddTexture(const Texture &tex);

	Mesh_info* CreateMesh(const uint &n_vertex, const float* vertex, const uint &n_index, const uint* index, const float* normals, const float* texCoords);
	Mesh_info* CreateMesh(const aiMesh *mesh);

	bool CreatePrimitive(const Primitives &type, const Object_data &data);

	void FillNormals(Mesh_info * ret, const float * normals = nullptr);
	void FillIndex(Mesh_info * ret, const uint & n_index, const aiFace* faces);
	void FillIndex(Mesh_info * ret, const uint & n_index, const uint* index);
	void FillVertex(Mesh_info * ret, const uint & n_vertex, const float* vertex);
	void FillTextureCoords(Mesh_info* mesh, const float* textureCoords);
	
	void Demo();

	const std::vector<ComponentMesh*>* GetObjects() const;
	std::vector<Texture>* GetTextures();
	const Texture* GetTexture(const int &id) const;
	/*Return Selected texture*/
	Texture* GetTexture() const;

	void SelectTexture(Texture &tex);
	// =========================================================== !old

private:
	GameObject* root = nullptr;

	// ==========================================================
	
	std::vector<ComponentMesh*> objects;

	std::vector<Texture> textures; //TEMPORARY
	Texture* tex_select = nullptr;

	// ========================================================
};

