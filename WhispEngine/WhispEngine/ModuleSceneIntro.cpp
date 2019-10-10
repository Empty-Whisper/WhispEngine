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
	if (show_grid)
		DrawGrid(50);	

	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawGrid(int width)
{
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	for (int i = -width; i <= width; ++i)
	{

		glVertex3f(i, 0, -width);
		glVertex3f(i, 0, width);

		glVertex3f(width, 0, i);
		glVertex3f(-width, 0, i);
		
	}
	glEnd();
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	glDeleteTextures(1, &id);

	return true;
}