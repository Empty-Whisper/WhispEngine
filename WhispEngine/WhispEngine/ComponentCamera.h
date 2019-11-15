#pragma once
#include "Component.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"


struct FrustumInfo
{
	float3 up_right = float3(0, 0, 0);
	float3 up_left = float3(0, 0, 0);
	float3 down_right = float3(0, 0, 0);
	float3 down_left = float3(0, 0, 0);

	float width = 0;
	float height = 0;

};

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* parent);
	~ComponentCamera();

public:

	void Update();
	void OnInspector();
	void DrawFrustum();
	void CalculateZNear(const float f_near);
	void CalculateZFar(const float f_far);

public:
	FrustumInfo zFar;
	FrustumInfo zNear;
	Frustum frustum;

private:
	float3 f_center = float3(0, 0, 0);
	float f_initial_z = 3;
	float f_depth = 10;
	float f_fov = 1;
	float f_aspect = 5;

};

