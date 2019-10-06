#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"
#include "GameObject.h"

//MathGeoLib--------------------------------------------------------
#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/lib_x86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "MathGeoLib/lib_x86/Release/MathGeoLib.lib")
#endif
//--------------------------------------------------------------------


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name.assign("SceneIntro");
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
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{

	DrawGrid(50);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawGrid(int width)
{
	glBegin(GL_LINES);
	for (int i = -width; i <= width; ++i)
	{

		glVertex3f(i, 0, -width);
		glVertex3f(i, 0, width);

		glVertex3f(width, 0, i);
		glVertex3f(-width, 0, i);

		glColor3f(1.f, 1.f, 1.f);

	}
	glEnd();
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	delete cube;
	delete w;

	return true;
}