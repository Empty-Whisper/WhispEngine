#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 3000.0f
#define MAX_DECCELERATION -4000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define CAMERA_OFFSET_X 10
#define CAMERA_OFFSET_Y 5
#define CAMERA_OFFSET_Z 10

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void NitroSpeed();
	bool SaveGhostData(bool to_save = true);
	void IterateGhost();
	void SetCheckpointPosition();
	void Reset();

public:

	PhysVehicle3D* vehicle = nullptr;
	PhysVehicle3D* ghost = nullptr;
	float turn = 0.0F;
	float acceleration = 0.0F;
	float brake = 0.0F;
	bool nitro = true;
	int checkpoint_value = 0;

private:
	vec3 direction = { 0,0,0 };
	vec3 axis = { 0,0,0 };
	bool accelerating = false;
	bool decelerating = false;
	p2DynArray<btTransform> ghost_pos;
	p2DynArray<btTransform> ghost_pos_prev;
	bool save_ghost_data = false;
	bool path_ghost = false;
	int iterator_ghost = 0;
	Timer timer_save_ghost;
	Timer timer_iterate_ghost;
	Timer timer_saving_data;
	btTransform checkpoint_vehicle_transform;

	int current_time = 0;
	int start_time = 0;
	int start_nitro = 0;

public:

	uint fx_horn = 0;
	uint fx_crash = 0;
	uint fx_racing = 0;
	uint fx_nitro_pick_up = 0;
	uint fx_nitro = 0;
	uint fx_car_engine = 0;
	uint fx_start_car = 0;
	uint fx_checkpoint = 0;
	bool turn_on_car = true;
};