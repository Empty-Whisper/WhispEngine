#pragma once
#include "Module.h"
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

	update_status Update();
	bool CleanUp();

	void AddObject(GameObject* obj);
	bool CreatePrimitive(const Primitives &type, const Object_data &data);
	void Demo();

	const std::vector<GameObject*>* GetObjects() const;

private:
	
	std::vector<GameObject*> objects;
};

