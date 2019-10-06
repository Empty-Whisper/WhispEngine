#include "ModuleObjectManager.h"
#include "glew/glew.h"



ModuleObjectManager::ModuleObjectManager()
{
}


ModuleObjectManager::~ModuleObjectManager()
{
}

update_status ModuleObjectManager::Update()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	for (auto i = objects.begin(); i != objects.end(); ++i) {
		(*i)->Draw();
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}

bool ModuleObjectManager::CleanUp()
{
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		delete *i;
	}
	objects.clear();

	return true;
}

void ModuleObjectManager::AddObject(GameObject * obj)
{
	objects.push_back(obj);
}
