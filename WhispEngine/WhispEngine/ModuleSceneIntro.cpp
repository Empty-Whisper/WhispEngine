#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"
#include "ComponentMesh.h"
#include "Brofiler/Brofiler.h"
#include "ModuleImport.h"

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

	GenerateGrid(10);

	App->importer->ImportFbx("Assets/Models/BakerHouse.fbx");
	
	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{

	if (App->renderer3D->is_rendering_scenene)
	{
		//Start Buffer Frame ----------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, App->renderer3D->scene_viewport->frame_buffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1, 1.f);
		//-----------------------------------------------------
	}
	else
	{
		//Start Buffer Frame ----------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, App->renderer3D->game_viewport->frame_buffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1, 1.f);
		//-----------------------------------------------------
	}

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update()
{
	BROFILER_CATEGORY("Scene", Profiler::Color::Orange);
	if (show_grid)
		DrawGrid();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawGrid()
{
	glDisable(GL_LIGHTING);

	glColor3f(1.f, 1.f, 1.f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, grid_id);
	glVertexPointer(3, GL_INT, 0, NULL);
	glDrawArrays(GL_LINES, 0, grid_vertex_size);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	glDeleteBuffers(1, &grid_id);

	return true;
}

void ModuleSceneIntro::GenerateGrid(const int & width)
{
	grid_vertex_size = (width + 1) * 4 * 3; // width + 1(for the middle line) * 4(4 points by line) * 3(3 numbers per point)
	int * grid = new int[grid_vertex_size];

	for (int i = 0; i <= width *3*4; i += 12)
	{
		grid[i]		= i / 6 - width; grid[i + 1] = 0; grid[i + 2] =  - width;
		grid[i + 3] = i / 6 - width; grid[i + 4] = 0; grid[i + 5] =    width;

		grid[i + 6] =     width; grid[i + 7]  = 0; grid[i + 8]  = i / 6 - width;
		grid[i + 9] =   - width; grid[i + 10] = 0; grid[i + 11] = i / 6 - width;
	}
	glGenBuffers(1, &grid_id);
	glBindBuffer(GL_ARRAY_BUFFER, grid_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * grid_vertex_size, grid, GL_STATIC_DRAW);
	
	delete[] grid;
}
