#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

struct zFrustrumFace
{
	float3 up_right = float3(0, 0, 0);
	float3 up_left = float3(0, 0, 0);
	float3 down_right = float3(0, 0, 0);
	float3 down_left = float3(0, 0, 0);

	float width = 0;
	float height = 0;
	
};

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
	void FocusObject(vec3 newPos);
	void Move(const vec3 &Movement);
	void MoveCameraByMouse(vec3 newPos, float speed);
	void MoveCameraOffsetByMouse(vec3 newPos, float speed);
	float* GetViewMatrix();
	void DrawFrustrumDebug();

private:

	void CalculateViewMatrix();
	vec3 CalculateMouseMotion();
	void ResetIsMovingCamera();
	const vec3 GetTransformPosition();

public:
	
	vec3 X, Y, Z, Position, Reference;

	float sensiblity = 0.f;
	int movement_speed = 0;
	int focus_speed = 0;
	int wheel_speed = 0;
	int offset_reference = 0;
	int slowness_middle_mouse = 0;
	int slowness_zoom_in_out = 0;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

	bool is_focusing = false;
	bool is_moving_camera = false;

	zFrustrumFace zFar;
	zFrustrumFace zNear;
	float3 f_center = float3(0, 0, 0);
	float f_initial_z = 0;
	float f_depth = 0;


};