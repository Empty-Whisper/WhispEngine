#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"

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
	
	

	int i = 0;
	int j = 2;

	float vertices[18]{
		i,i,i,
		j,i,i,
		i,j,i,

		j,i,i,
		j,j,i,
		i,j,i
	};

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);

	

	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);



	glBegin(GL_LINES);
	for (int i = 0; i <= 100; i+=2)
	{
		
		glVertex3f(i, 0, 0);
		glVertex3f(i, 0, 100);

		glVertex3f(100, 0, i);
		glVertex3f(0, 0, i);



	}	
	glEnd();
	glLineWidth(1.0f);

	/*glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(0, 2, 0);

	glVertex3f(2, 0, 0);
	glVertex3f(2, 2, 0);
	glVertex3f(0, 2, 0);

	
	

	glColor3f(0.f, 0.f, 1.f);


	glEnd();*/


	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}