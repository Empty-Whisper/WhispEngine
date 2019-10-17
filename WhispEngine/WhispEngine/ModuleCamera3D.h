#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	//void LookAround(const math::float3 &Reference, float DeltaX, float DeltaY);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void MoveCameraByMouse(vec3 DirectionA, vec3 DirectionB);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();
	void ResetIsMovingCamera();

public:
	
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	float sensiblity = 0.f;
	float movement_speed = 0.f;
	float wheel_speed = 0.f;
	float offset_reference = 0.f;
	bool alt = false;
	bool is_focusing = false;
	bool is_moving_camera = false;


};