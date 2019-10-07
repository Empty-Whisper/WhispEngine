#include "ModuleObjectManager.h"
#include "glew/glew.h"
#include "Application.h"



ModuleObjectManager::ModuleObjectManager()
{
	name.assign("ObjectManager");
}


ModuleObjectManager::~ModuleObjectManager()
{
}

update_status ModuleObjectManager::Update()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(1.5f);
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		if ((*i)->active) {

			if (App->renderer3D->fill) {
				glColor3fv((*i)->color);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				(*i)->Draw();
			}
			if (App->renderer3D->wireframe) {
				glColor3fv((*i)->wire_color);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				(*i)->Draw();
			}

			(*i)->vertex_normals = App->renderer3D->my_normals;
			if (App->renderer3D->see_normals)
				(*i)->DrawNormals();
		}
	}
	glLineWidth(1.0f);
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

const std::vector<GameObject*>* ModuleObjectManager::GetObjects() const
{
	return &objects;
}
