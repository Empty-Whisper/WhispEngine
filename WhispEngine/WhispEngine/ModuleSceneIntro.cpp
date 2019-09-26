#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include <random>
#include "PCG/pcg_random.hpp"

#include "Imgui/imgui.h"

#include "JSON/json.hpp"
#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>

#include "HardwareInfo.h"

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
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	//HARDWARE ----------------------------------------------------------------------
	ImGui::Begin("Hardware");
	ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetCPUCount());
	ImGui::Text("Cache:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetCPUCacheLineSize());
	ImGui::Text("RAM:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetSystemRAM());
	ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::Text(App->hardware->GetSDLVersion());
	ImGui::Text("OpenGL Version:"); ImGui::SameLine(); ImGui::Text(App->hardware->GetOpenGLVersion());
	ImGui::Text("Graphics Card Company:"); ImGui::SameLine(); ImGui::Text(App->hardware->GetGraphicsCardVendor());
	ImGui::Text("Graphics Card Model:"); ImGui::SameLine(); ImGui::Text(App->hardware->GetGraphicsCardModel());

	ImGui::Text("VRAM Budget:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetTotalAvailableMemory());/*TODO: Names are correct?*/
	ImGui::Text("VRAM Usage:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetCurrentMemory());
	ImGui::Text("VRAM Available:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetDedicatedMemory());
	ImGui::Text("VRAM Reserved:"); ImGui::SameLine(); ImGui::Text("%i", App->hardware->GetTotalMemoryEvicted());
	ImGui::End();
	//-------------------------------------------------------------------

	
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}