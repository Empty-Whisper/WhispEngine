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

	const float3 GetPosition();

	//Setters
	void SetNearZ(const float &zNear);
	void SetFarZ(const float &zFar);
	void SetFOV(const float &fov);
	void SetAspectRatio(const float &ratio);

	void SetTransformPosition(const float3 &pos);
	void SetVectorDirectionFront(const float3 &pos);
	void SetVectorDirectionUp(const float3 &pos);

	//Culling
	void DrawInsideFrustum();
	bool BboxIntersectsFrustum(const AABB& box);

public:
	bool	frustum_culling = true;
	bool	is_main_camera = false;
private:
	Frustum frustum;
	float	aspect_ratio = 0.0f;
	float   vertical_fov = 0.0f;
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
	Camera* GetCurrentCamera();
	void SetCurrentCamera(Camera* camera);

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
	Camera* editor_camera = nullptr;
	Camera* current_camera = nullptr;
	std::vector<Camera*> cameras;


	mat4x4 ViewMatrix, ViewMatrixInverse;

	bool is_focusing = false;
	bool is_moving_camera = false;

};