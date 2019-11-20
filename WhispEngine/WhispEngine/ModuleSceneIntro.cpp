#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"
#include "ComponentMesh.h"
#include "Brofiler/Brofiler.h"

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
	octree = new OctreeTree(float3(-100, -100, -100), float3(100, 100, 100), 1);
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

	GenerateGrid(10);

	App->importer->Import("Assets/Models/BakerHouse.fbx");
	App->importer->Import("Assets/Models/BakerHouse.fbx.meta");

	scene_name.assign("SampleScene");

	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{
	//Start Buffer Frame ----------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, App->renderer3D->frame_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.1, 1.f);
	//-----------------------------------------------------

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleSceneIntro::Update()
{
	BROFILER_CATEGORY("Scene", Profiler::Color::Orange);
	if (show_grid)
		DrawGrid();

	if (show_octree) {
		glDisable(GL_LIGHTING);
		octree->Render();
		glEnable(GL_LIGHTING);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{
	// Start Buffer Frame ----------------------------------
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, App->renderer3D->render_texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	// -----------------------------------------------------

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

	if (octree != nullptr)
		delete octree;

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

bool ModuleSceneIntro::SaveScene()
{
	bool ret = true;

	nlohmann::json scene;

	ret = App->object_manager->SaveGameObjects(scene[scene_name.c_str()]);

	App->dummy_file_system->SaveFile((ASSETS_FOLDER + scene_name + ".scene").c_str(), scene);

	return ret;
}

bool ModuleSceneIntro::LoadScene() const
{
	bool ret = true;

	nlohmann::json scene = App->dummy_file_system->OpenFile("Assets/SampleScene.scene");

	auto it = scene.begin();
	ret = App->object_manager->LoadGameObjects((*it)["GameObjects"]);

	return ret;
}
