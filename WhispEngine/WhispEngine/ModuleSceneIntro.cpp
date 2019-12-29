#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"

#include <string>

#include "Imgui/imgui.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Brofiler/Brofiler.h"
#include "ModuleImport.h"
#include "ModuleObjectManager.h"
#include "ModuleResources.h"
#include "ModuleGUI.h"
#include "PanelConfiguration.h"

//MathGeoLib--------------------------------------------------------
#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/lib_x86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "MathGeoLib/lib_x86/Release/MathGeoLib.lib")
#endif
//--------------------------------------------------------------------

#include "mmgr/mmgr.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name.assign("SceneIntro");
#ifndef GAME_BUILD
	octree = new OctreeTree(float3(-100, -100, -100), float3(100, 100, 100), 1);
#else
	show_grid = false;
	show_octree = false;
	show_mouse_raycast = false;
#endif
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	GenerateGrid(10);

	LoadScene("Assets/Scenes/TankLua.scene");

	//Skybox
	App->resources->Get(App->file_system->GetUIDFromMeta("Assets/Models/Skybox.fbx.meta"))->LoadToMemory();

	std::vector<GameObject*> game_objects;
	App->object_manager->GetChildsFrom(App->object_manager->root, game_objects);

	for (GameObject* it : game_objects)
	{
		//Skybox
		if (strncmp(it->GetName(), skybox_id.c_str(), skybox_id.length()) == 0)
		{
			SkyboxObject = it;
			((ComponentTransform*)SkyboxObject->GetComponent(ComponentType::TRANSFORM))->SetScale(7000, 7000, 7000);
			SkyboxObject->parent->Detach();
			SkyboxObject->Detach();
		}
	}
	
	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{
	BROFILER_CATEGORY("Scene", Profiler::Color::Orange);
	if (App->renderer3D->is_rendering_scene)
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

#ifndef GAME_BUILD
	if (App->gui->config->active_skybox)
		SkyboxObject->Update();
		
#else
	SkyboxObject->Update();

#endif
	
			
	if (show_octree) {
		glDisable(GL_LIGHTING);
		octree->Render();
		glEnable(GL_LIGHTING);
	}

	if (show_mouse_raycast) {
		glDisable(GL_LIGHTING);
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		glVertex3f(mouse_pick_0.x, mouse_pick_0.y, mouse_pick_0.z);
		glVertex3f(mouse_pick_1.x, mouse_pick_1.y, mouse_pick_1.z);
		glEnd();
		glEnable(GL_LIGHTING);
	}

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

void ModuleSceneIntro::DrawSkyboxSphere()
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

void ModuleSceneIntro::GenerateSkyboxSphere(const int & radius)
{
}

bool ModuleSceneIntro::SaveCurrentScene()
{
	bool ret = true;

	nlohmann::json scene;

	if (scene_path.empty()) {
		scene_path.assign("Assets/Scenes/Sample.scene");
	}
	std::string name = App->file_system->GetFileNameFromPath(scene_path.c_str());
	
	Camera* cam = App->camera->GetSceneCamera();
	App->json->AddFloat3("position", cam->GetPosition(), scene["Camera"]);
	App->json->AddQuaternion("rotation", cam->GetRotation(), scene["Camera"]);

	ret = App->object_manager->SaveGameObjects(scene);

	if (App->file_system->GetFormat(scene_path.c_str()) != FileSystem::Format::SCENE)
		scene_path.append(".scene");

	App->file_system->SaveFile(scene_path.c_str(), scene);

	return ret;
}

bool ModuleSceneIntro::SaveTemporaryScene() const
{
	bool ret = true;

	nlohmann::json scene;

	Camera* cam = App->camera->GetSceneCamera();
	App->json->AddFloat3("position", cam->GetPosition(), scene["Camera"]);
	App->json->AddQuaternion("rotation", cam->GetRotation(), scene["Camera"]);

	ret = App->object_manager->SaveGameObjects(scene);

	App->file_system->RemoveFile("Assets/Scenes/temporaryScene.scene");
	App->file_system->SaveFile("Assets/Scenes/temporaryScene.scene", scene);

	int attr = GetFileAttributes("Assets/Scenes/temporaryScene.scene");
	if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
		SetFileAttributes("Assets/Scenes/temporaryScene.scene", attr | FILE_ATTRIBUTE_HIDDEN);
	}

	return ret;
}

bool ModuleSceneIntro::LoadTemporaryScene()
{
	bool ret = true;

	nlohmann::json scene_file = App->file_system->OpenFile("Assets/Scenes/temporaryScene.scene");

	ret = App->object_manager->LoadGameObjects(scene_file["GameObjects"]);
	App->object_manager->LoadScripts(scene_file["GameObjects"]);

	App->file_system->RemoveFile("Assets/Scenes/temporaryScene.scene");

	return ret;
}

bool ModuleSceneIntro::LoadScene(const char* scene)
{
	bool ret = true;

	nlohmann::json scene_file = App->file_system->OpenFile(scene);
	
	Camera* cam = App->camera->GetSceneCamera();
	if (cam != nullptr) {
		cam->SetTransformPosition(App->json->GetFloat3("position", scene_file["Camera"]));
		Quat r = App->json->GetQuaternion("rotation", scene_file["Camera"]);
		cam->SetVectorDirectionFront(-r.WorldZ());
		cam->SetVectorDirectionUp(r.WorldY());
	}

	scene_path.assign(scene);

	ret = App->object_manager->LoadGameObjects(scene_file["GameObjects"]);
	App->object_manager->LoadScripts(scene_file["GameObjects"]);

	if (octree != nullptr)
		octree->Recalculate();

	return ret;
}

GameObject* ModuleSceneIntro::LoadPrefab(const char * prefab)
{
	nlohmann::json object = App->file_system->OpenFile(prefab);

	GameObject* ret = App->object_manager->LoadGameObject(*(object["GameObjects"]).begin(), App->object_manager->root);
	App->object_manager->RefreshObjectsUIDMap();
	App->object_manager->LoadScripts(object["GameObjects"]);

	return ret;
}

bool ModuleSceneIntro::CreateEmptyScene(const char * path)
{
	App->object_manager->ResetObjects();
	scene_path.assign(path);

	return SaveCurrentScene();
}

void ModuleSceneIntro::DebugOctree()
{
	if (App->file_system->Exists("Assets/Scenes/Octree.scene")) {
		LoadScene("Assets/Scenes/Octree.scene");
		show_octree = true;
	}
	else {
		LOG("Assets/Scenes/Octree.scene does not exist");
	}
}

std::string ModuleSceneIntro::GetSceneName() const
{
	return App->file_system->GetFileNameFromPath(scene_path.c_str());
}
