#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Timer.h"
#include "PhysBody3D.h"

#define MAX_SNAKE 2

struct PhysMotor3D;

enum class ORIENTATION {
	NORTH,		//positive Z axis
	SOUTH,		//negative Z axis
	EAST,		//negative X azis
	WEST,		//positive X axis

	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void CreateRect(const float &x, const float &y, const float &z, const float &width, const float &length, const Cube & cube, ORIENTATION orientation = ORIENTATION::NORTH);
	void CreateCurve(const float &x, const float &y, const float &z, const float & width, const float & angle_i, const float & angle_f, const Cube & cube, const float & radius);
	void CreatePendulum(const float &x, const float &z);
	//void CreateRamp(const float & x, const float & y, const float & z, const float & inclination = 10.0f, const float& rotation = 0.0f);

	void NitroObject(vec3 pos, int size, int distance_to);
	void PickUpNitroObject(PhysBody3D* nitro_body);

	void CreateCheckpoint(vec3 pos, bool rotate, PhysBody3D::Tag type = PhysBody3D::Tag::CHECKPOINT);
	void Checkpoint(PhysBody3D* checkpoint_body);

	void ResetVars();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	//void Angles2AxisAngle(const float& alpha_x, const float& beta_y, vec3& u, float& angle);

public:
	bool camera_free = false;
	p2DynArray <Cube*> map;

	p2DynArray <Sphere*> nitro_objects;
	p2DynArray <PhysBody3D*> nitro_objects_body;

	
	p2DynArray <Cube*> checkpoint_objects;
	p2DynArray <PhysBody3D*> checkpoint_objects_body;

	p2DynArray<Sphere*> pendulumBall_shape;
	p2DynArray<PhysBody3D*> pendulumBall_body;

	bool game_over = false;

protected:
	Timer timer_lap;

private:
	Timer timer;	
	int current_time = 0;
	int start_time = 0;
	int checkpoints = 0;
	int laps = 0;
	bool firstLap = true;
	char win_string[100];
	int fastest_lap = 0;
	int fastest_lap_time = 0;
	bool start_game = false;
	bool game_win = false;

};