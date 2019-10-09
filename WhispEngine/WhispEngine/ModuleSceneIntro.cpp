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

	
	GLubyte checkImage[100][100][4];
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	if (show_grid)
		DrawGrid(50);

	static const GLfloat c[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
	};
	

	glBindTexture(GL_TEXTURE_2D, id);
	//glTexCoord2f(1, 0);
	//glTexCoordPointer(2, GL_UNSIGNED_BYTE, sizeof(char) * CHECKERS_HEIGHT*CHECKERS_WIDTH * 4, NULL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glBegin(GL_TRIANGLES);
	//for (int i = 0; i < 108; i+=9) {
	//	/*glTexCoord2f(0.0, 0.0); glVertex3f(c[i], c[i + 1], c[i + 2]);
	//	glTexCoord2f(1, 0.0); glVertex3f(c[i+3], c[i + 4], c[i + 5]);
	//	glTexCoord2f(0.0, 1.0); glVertex3f(c[i+6], c[i + 7], c[i + 8]);

	//	glTexCoord2f(1.0, 0.0); glVertex3f(c[i+9], c[i + 10], c[i + 11]);
	//	glTexCoord2f(1.0, 1.0); glVertex3f(c[i + 12], c[i + 13], c[i + 14]);
	//	glTexCoord2f(1.0, 1.0); glVertex3f(c[i + 15], c[i + 16], c[i + 17]);*/
	//	glTexCoord2f(0, 0); glVertex3f(c[i], c[i + 1], c[i + 2]);
	//	glTexCoord2f(1, 0); glVertex3f(c[i + 3], c[i + 4], c[i + 5]);
	//	glTexCoord2f(0, 1); glVertex3f(c[i + 6], c[i + 7], c[i + 8]);

	//	// second triangle, top right half
	//	/*glVertex3f(1, 0, 0);
	//	glTexCoord2f(1, 1); glVertex3f(1, 1, 0);
	//	glTexCoord2f(0, 1); glVertex3f(0, 1, 0);*/
	//}
	//glEnd();

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

	return true;
}