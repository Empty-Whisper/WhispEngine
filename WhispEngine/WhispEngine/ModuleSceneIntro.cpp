#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include <random>
#include "PCG/pcg_random.hpp"

#include "Imgui/imgui.h"

#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/lib_x86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "MathGeoLib/lib_x86/Release/MathGeoLib.lib")
#endif


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
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


	//MathGeoLib test -----------------------------------------------
	math::Sphere *a = new math::Sphere(math::float3(0.f, 0.f, 0.f), 5);
	math::Sphere *b = new math::Sphere(math::float3(0.f, 0.f, 0.f), 5);
	math::Sphere *c = new math::Sphere(math::float3(10.f, 10.f, 10.f), 1);

	if (a->Intersects(*b)) {
		a_inter_b = "A has intersection with B";
	}
	else {
		a_inter_b = "A has no intersection with B";
	}
	
	if (b->Intersects(*c)) {
		b_inter_c = "B has intersection with C";
	}
	else {
		b_inter_c = "B has no intersection with C";
	}

	if (a->Intersects(*c)) {
		a_inter_c = "A has intersection with C";
	}
	else {
		a_inter_c = "A has no intersection with C";
	}

	timer.Start();
	//------------------------------------------------------------------

	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{

	//PCG test --------------------------------------------------------
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	pcg32 rng(seed_source);
	std::uniform_int_distribution<int> uniform_dist_int(0, 10);
	std::uniform_real_distribution<float> uniform_dist_f(0, 1);

	if (timer.Read() >= 4000) {
		inum = uniform_dist_int(rng);
		fnum = uniform_dist_f(rng);
		timer.Start();
	}
	ImGui::Begin("Random number");
	ImGui::Text("In"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 0, 0, 1), "%i", 3 - timer.Read() / 1000);
	ImGui::SameLine(); ImGui::Text("seconds will generate a random number between 0 and 10 and a float between 0 and 1");
	ImGui::Text("Int: %i", inum);
	ImGui::Text("Float: %f", fnum);
	ImGui::End();

	//----------------------------------------------------------------------
	ImGui::Begin("Spheres intersections");
	ImGui::Text("A is in (0, 0, 0) and has rad = 5");
	ImGui::Text("\nB is in (0, 0, 0) and has rad = 5");
	ImGui::Text("\nC is in (10, 10, 10) and has rad = 1");
	ImGui::NewLine();
	ImGui::Text(a_inter_b);
	ImGui::Text(b_inter_c);
	ImGui::Text(a_inter_c);
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