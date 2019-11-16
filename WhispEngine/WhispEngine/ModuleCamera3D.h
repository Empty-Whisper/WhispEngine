#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class Camera
{
public: 
	Camera();

	//Getters
	Frustum GetFrustum();
	const float GetNearZ() const;
	const float GetFarZ() const;
	const float GetVerticalFOV() const;
	const float4x4 GetViewMatrix() const;
	const float4x4 GetProjectionMatrix() const;
	void GetAllCorners(float3* corners);

	//Setters
	void SetNearZ(const float& set);
	void SetFarZ(const float& set);
	void SetFOV(const float& set);
	void SetAspectRatio(const float& set);

	


private:
	Frustum frustum;
	float	aspect_ratio = 0.0f;
	float   vertical_fov = 0.0f;
	bool	frustum_culling = true;
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
	void LookAt(const vec3 &Spot);
	void FocusObject(vec3 newPos);
	void Move(const vec3 &Movement);
	void MoveCameraByMouse(vec3 newPos, float speed);
	void MoveCameraOffsetByMouse(vec3 newPos, float speed);
	float* GetViewMatrix();
	
	Camera* CreateCamera();
	Camera* GetGameCamera();

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
	Camera* game_camera = nullptr;
	Camera* scene_camera = nullptr;
	std::vector<Camera*> cameras;


	mat4x4 ViewMatrix, ViewMatrixInverse;

	bool is_focusing = false;
	bool is_moving_camera = false;

};