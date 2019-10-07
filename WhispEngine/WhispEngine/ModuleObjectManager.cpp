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

	if (data.rotate.axis[0] != 0 || data.rotate.axis[1] != 0 || data.rotate.axis[2] != 0) {
		float mgn = std::sqrt(data.rotate.axis[0] * data.rotate.axis[0] + data.rotate.axis[1] * data.rotate.axis[1] + data.rotate.axis[2] * data.rotate.axis[2]); // normalize rotation axis
		float rot[3] = { data.rotate.axis[0] / mgn,data.rotate.axis[1] / mgn ,data.rotate.axis[2] / mgn };
		par_shapes_rotate(mesh, data.rotate.angle, rot);
	}

	par_shapes_scale(mesh, data.scale.x, data.scale.y, data.scale.z);

	GameObject* obj = new GameObject(mesh->npoints, mesh->points, mesh->ntriangles * 3, mesh->triangles, mesh->normals);
	obj->SetColors(data.face_color, data.wire_color);
	objects.push_back(obj);

	par_shapes_free_mesh(mesh);

	return ret;
}

void ModuleObjectManager::Demo()
{
	std::vector<par_shapes_mesh*> mesh;
	mesh.resize((int)Primitives::MAX);
	mesh[0] = par_shapes_create_cube();
	mesh[1] = par_shapes_create_tetrahedron();
	mesh[2] = par_shapes_create_octahedron();
	mesh[3] = par_shapes_create_dodecahedron();
	mesh[4] = par_shapes_create_icosahedron();
	mesh[5] = par_shapes_create_subdivided_sphere(3);
	mesh[6] = par_shapes_create_hemisphere(10, 10);
	mesh[7] = par_shapes_create_torus(10, 10, 0.5f);
	mesh[8] = par_shapes_create_cone(10, 10);
	mesh[9] = par_shapes_create_cylinder(10, 10);

	int posx = -10;
	float color[3] = {1,1,1};
	for (auto i = mesh.begin(); i != mesh.end(); ++i) {
		par_shapes_translate(*i, posx, 0.f, 3);
		GameObject* obj = new GameObject((*i)->npoints, (*i)->points, (*i)->ntriangles * 3, (*i)->triangles, (*i)->normals);
		obj->SetColors(color);
		objects.push_back(obj);
		par_shapes_free_mesh((*i));
		posx += 3;
	}

	mesh.clear();	
}

const std::vector<GameObject*>* ModuleObjectManager::GetObjects() const
{
	return &objects;
}
