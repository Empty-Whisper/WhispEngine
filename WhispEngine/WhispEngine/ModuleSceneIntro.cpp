#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"
#include "GameObject.h"

// Devil ---------------------------------------------------------
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "DevIL/lib/x86/unicode/Release/DevIL.lib")
#pragma comment (lib, "DevIL/lib/x86/unicode/Release/ILU.lib")
#pragma comment (lib, "DevIL/lib/x86/unicode/Release/ILUT.lib")
//--------------------------------------------------------------------

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

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	

	ilInit();

	ILuint devilID;


	ilGenImages(1, &devilID);
	ilBindImage(devilID);
	ilutRenderer(ILUT_OPENGL);  // Switch the renderer
	if (ilLoad(IL_DDS, "Lenna.dds") == IL_TRUE) {
		LOG("OPEN LENA");
	}
	else {
		LOG("CANNOT OPEN LENA");
	}

	/*if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		LOG("FAILED TO CONVERT");
	if (!ilTexImage(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, 1, GL_RGBA, IL_UNSIGNED_BYTE, ilGetData()))
		LOG("AYAYAYA");*/
	LOG("%u",ilutGLBindTexImage());

	glBindTexture(GL_TEXTURE_2D, ilutGLBindTexImage());

	/*unsigned char* tex = new unsigned char[ilGetInteger(IL_IMAGE_WIDTH) * ilGetInteger(IL_IMAGE_HEIGHT)];
	memcpy(tex, ilGetData(), ilGetInteger(IL_IMAGE_WIDTH) * ilGetInteger(IL_IMAGE_HEIGHT));
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE, tex);*/
	//Delete file from memory
	ilDeleteImages(1, &devilID);


	
	
	GLuint openglID;

	//openglID = ilutGLBindTexImage(); // This generates the texture for you


	
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

	return true;
}