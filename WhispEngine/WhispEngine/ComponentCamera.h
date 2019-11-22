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
  
	void Save(nlohmann::json &node);

private:
	Camera* camera = nullptr;
public:
	bool	checkbox_main_camera = false;
	bool	is_main_camera = true;
};

