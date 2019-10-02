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

	m = par_shapes_create_cube();

	glGenBuffers(1, &shape);
	glBindBuffer(GL_ARRAY_BUFFER, shape);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->npoints * 3, m->points, GL_STATIC_DRAW);

	glGenBuffers(1, &shape2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*m->ntriangles * 3, m->triangles, GL_STATIC_DRAW);

	for (int i = 0; i < m->npoints*3; i++) {
		if (i % 3 == 0) LOG("\n");
		LOG("%f", m->points[i]);
	}
	LOG("\n");
	for (int i = 0; i < m->ntriangles; i++) {
		LOG("%i", m->triangles[i]);
	}
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	glColor3f(1.f, 0.f, 0.f);
	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, shape);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape2);
	// draw a cube
	glDrawElements(GL_TRIANGLES, m->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}