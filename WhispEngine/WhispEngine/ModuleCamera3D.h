#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

enum CameraMovementType
{
	FRONT,
	BACK,
	RIGHT,
	LEFT,
	UP,
	DOWN,
};

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

	//Camera - Editor
	void Movement(CameraMovementType type, const float& speed);
	void CameraViewRotation(const float2& pos);
	void MulQuat(math::Quat& quat);
	void FocusObject(const AABB& aabb);
	void OrbitObject(const float3& center, const float2& pos);

	void Look(const float3& look_pos);

	//Culling
	void DrawInsideFrustum();
	bool BboxIntersectsFrustum(const AABB& box);

public:
	bool	frustum_culling = true;
	bool	is_main_camera = false;
	bool	is_focusing = false;
	int		offset_reference_focus = 0;
	int		focus_speed = 0;


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
	
	Camera* CreateCamera();
	void DeleteCamera(Camera* camera);
	void DeleteVectorCameras();
	Camera* GetEditorCamera();
	Camera* GetCurrentCamera();
	void SetCurrentCamera(Camera* camera);

private:

	void MoveCameraOutScene();

public:

	float sensiblity = 0.f;
	int movement_speed = 0;
	int focus_speed = 0;
	int wheel_speed = 0;
	int offset_reference = 0;
	int slowness_middle_mouse = 0;
	int slowness_zoom_in_out = 0;

public:
	Camera* editor_camera = nullptr;
	Camera* current_camera = nullptr;
	std::vector<Camera*> cameras;
	bool is_moving_camera = false;

};