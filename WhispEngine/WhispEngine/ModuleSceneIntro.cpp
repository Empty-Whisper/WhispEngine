#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"

//test
#include "MathGeoLib/include/MathGeoLib.h"

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

	//plane = new Plane();
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

	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	//plane->Render();
	
	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}