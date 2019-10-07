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

bool ModuleObjectManager::CreatePrimitive(const Primitives & type, const Object_data &data)
{
	bool ret = true;

	par_shapes_mesh* mesh = nullptr;

	switch (type)
	{
	case Primitives::CUBE:
		mesh = par_shapes_create_cube();
		break;
	case Primitives::TETRAHEDRON:
		mesh = par_shapes_create_tetrahedron();
		break;
	case Primitives::OCTAHEDRON:
		mesh = par_shapes_create_octahedron();
		break;
	case Primitives::DODECAHEDRON:
		mesh = par_shapes_create_dodecahedron();
		break;
	case Primitives::ICOSAHEDRON:
		mesh = par_shapes_create_icosahedron();
		break;
	case Primitives::SPHERE:
		//TODO Create a sphere with radious, rings and sectors, not by subdivisions https://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c/5989676#5989676
		mesh = par_shapes_create_subdivided_sphere(data.slices);
		break;
	case Primitives::HEMISPHERE:
		mesh = par_shapes_create_hemisphere(data.slices, data.rings);
		break;
	case Primitives::TORUS:
		mesh = par_shapes_create_torus(data.slices, data.rings, data.radius);
		break;
	case Primitives::CONE:
		mesh = par_shapes_create_cone(data.slices, data.rings);
		break;
	case Primitives::CYLINDER:
		mesh = par_shapes_create_cylinder(data.slices, data.rings);
		break;
	default:
		LOG("Primitive not found to create. id: %i", (int)type);
		break;
	}

	par_shapes_translate(mesh, data.pos.x, data.pos.y, data.pos.z);
	par_shapes_rotate(mesh, data.rotate.angle, data.rotate.axis);
	par_shapes_scale(mesh, data.scale.x, data.scale.y, data.scale.z);

	GameObject* obj = new GameObject(mesh->npoints, mesh->points, mesh->ntriangles * 3, mesh->triangles, mesh->normals);
	objects.push_back(obj);

	par_shapes_free_mesh(mesh);

	return ret;
}

const std::vector<GameObject*>* ModuleObjectManager::GetObjects() const
{
	return &objects;
}
