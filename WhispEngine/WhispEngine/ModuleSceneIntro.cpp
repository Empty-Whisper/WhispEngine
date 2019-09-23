#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include <random>
#include "PCG/pcg_random.hpp"

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

	plane = new prim::Plane();
	plane->axis = true;

	App->camera->Move(vec3(5.0f, 3.0f, 5.0f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	plane->Render();

	pcg_extras::seed_seq_from<std::random_device> seed_source;

	pcg32 rng(seed_source);
	std::uniform_int_distribution<int> uniform_dist_int(0, 10);
	std::uniform_real_distribution<float> uniform_dist_f(0, 1);


	for (int i = 0; i < 10; ++i) {
		LOG("%i", uniform_dist_int(rng));
		LOG("%f", uniform_dist_f(rng));
	}
	
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	delete plane;

	return true;
}