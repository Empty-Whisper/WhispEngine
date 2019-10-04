#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"
#include "GameObject.h"

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

	m = par_shapes_create_cube();
	cube = new GameObject(m->npoints, m->points, m->ntriangles, m->triangles);
	par_shapes_free_mesh(m);
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	glColor3f(1.f, 0.f, 0.f);
	// activate and specify pointer to vertex array

	glEnableClientState(GL_VERTEX_ARRAY);
	
	cube->Draw();

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);



	glBegin(GL_LINES);
	for (int i = 0; i <= 100; ++i)
	{

		glVertex3f(i, 0, 0);
		glVertex3f(i, 0, 100);

		glVertex3f(100, 0, i);
		glVertex3f(0, 0, i);

		glColor3f(1.f, 1.f, 1.f);

	}
	glEnd();
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	delete cube;

	return true;
}