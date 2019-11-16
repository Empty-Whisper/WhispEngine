#pragma once
#include "Component.h"
#include "Globals.h"
#include "glmath.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class GameObject;
class Camera;

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


private:
	Camera* camera = nullptr;
};

