#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include <random>
#include "PCG/pcg_random.hpp"

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

	plane = new prim::Plane();
	plane->axis = true;

	App->camera->Move(vec3(5.0f, 3.0f, 5.0f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));


	//MathGeoLib test -----------------------------------------------
	math::Sphere *a = new math::Sphere(math::float3(0.f, 0.f, 0.f), 5);
	math::Sphere *b = new math::Sphere(math::float3(0.f, 0.f, 0.f), 5);
	math::Sphere *c = new math::Sphere(math::float3(10.f, 10.f, 10.f), 1);

	if (a->Intersects(*b)) {
		LOG("A has intersection with B");
	}
	if (b->Intersects(*c)) {
		LOG("B has intersection with C");
	}
	else {
		LOG("B has no intersection with C");
	}
	//------------------------------------------------------------------

	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	plane->Render();

	//PCG test --------------------------------------------------------
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	pcg32 rng(seed_source);
	std::uniform_int_distribution<int> uniform_dist_int(0, 10);
	std::uniform_real_distribution<float> uniform_dist_f(0, 1);

	for (int i = 0; i < 10; ++i) {
		LOG("%i", uniform_dist_int(rng));
		LOG("%f", uniform_dist_f(rng));
	}
	//-------------------------------------------------------------------
	
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	delete plane;

	return true;
}