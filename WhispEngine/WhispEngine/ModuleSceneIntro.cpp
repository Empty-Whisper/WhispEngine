#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

//#ifndef PAR_SHAPES_IMPLEMENTATION
//#define PAR_SHAPES_IMPLEMENTATION
//#endif // !1
#include "par_shapes.h"

#include "Imgui/imgui.h"
//#include "par_shapes.h" //https://prideout.net/shapes

#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/lib_x86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "MathGeoLib/lib_x86/Release/MathGeoLib.lib")
#endif


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(5.0f, 3.0f, 5.0f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	//m = par_shapes_create_subdivided_sphere(5);
	//m = par_shapes_create_cube();
	//m = par_shapes_create_rock(30, 3);
	//m = par_shapes_create_torus(30, 14, 0.8f);
	//m = par_shapes_create_cone(4, 3); //Piramide
	m = par_shapes_create_cone(100, 3); //Cone
	//m = par_shapes_create_plane(1, 1);

	shapes.push_back(par_shapes_create_subdivided_sphere(5));
	shapes.push_back(par_shapes_create_rock(30, 3));
	shapes.push_back(par_shapes_create_torus(30, 14, 0.8f));
	shapes.push_back(par_shapes_create_cone(4, 3));
	shapes.push_back(par_shapes_create_cone(100, 3));
	shapes.push_back(par_shapes_create_plane(1, 1));

	for (auto i = shapes.begin(); i != shapes.end(); ++i) {
		glGenBuffers(1, &(*i).id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, (*i).id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (*i).mesh->npoints * 3, (*i).mesh->points, GL_STATIC_DRAW);

		glGenBuffers(1, &(*i).id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i).id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*(*i).mesh->ntriangles * 3, (*i).mesh->triangles, GL_STATIC_DRAW);
	}	
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	glColor3f(1.f, 0.f, 0.f);
	// activate and specify pointer to vertex array

	glEnableClientState(GL_VERTEX_ARRAY);
	
	for (auto i = shapes.begin(); i != shapes.end(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, (*i).id_vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i).id_index);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		// draw a cube
		glDrawElements(GL_TRIANGLES, (*i).mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	}

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (auto i = shapes.begin(); i != shapes.end(); ++i) {
		par_shapes_free_mesh((*i).mesh);
	}
	//par_shapes_free_mesh(m);
	return true;
}