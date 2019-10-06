#pragma once
#include "Module.h"
#include "GameObject.h"

class ModuleObjectManager :
	public Module
{
public:
	ModuleObjectManager();
	~ModuleObjectManager();

	update_status Update();
	bool CleanUp();

	void AddObject(GameObject* obj);

private:
	std::vector<GameObject*> objects;
};

