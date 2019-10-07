#pragma once
#include "Module.h"
#include "GameObject.h"

enum class Primitives {
	NONE = -1,
	CUBE, SPHERE,
	MAX
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
	bool CreatePrimitive(const Primitives &type);

	const std::vector<GameObject*>* GetObjects() const;

private:
	std::vector<GameObject*> objects;
};

