#pragma once
#include <string>
#include <vector>

#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

enum class ObjectSelected
{
	NONE = -1,
	SELECTED, // Orange outline
	CHILD_FROM_PARENT_SELECTED, // Blue outline
};

class GameObject
{
	friend class ComponentMesh;
public:
	GameObject(GameObject *parent);
	~GameObject();

public:
	void Update();
	void DrawBoundingBoxAABB();
	void DrawBoundingBoxOBB();
	Component* CreateComponent(const ComponentType &type);
	void	   DeleteComponent(Component* comp);
	Component* GetComponent(const ComponentType &type);
	bool	   HasComponent(const ComponentType &type);

	bool IsActive() const;
	void SetActive(const bool &to_active);

	const char* GetName() const;
	void SetName(const char* name);

	ObjectSelected GetSelect() const;
	void Select();
	void Deselect();

	void Detach();
	void Attach(GameObject* parent);
	bool HasChild(GameObject* child);

	void SetAABB(AABB& bbox);
	AABB GetAABB() const;
	void SetOBB(AABB& bbox);
	AABB GetOBB() const;

	void GenerateOBB(OBB obb, GameObject* &obj);


public:
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	GameObject* parent = nullptr;

private:
	bool active = true;
	std::string name;
	ObjectSelected obj_selected = ObjectSelected::NONE;
	std::vector<Component*> components_to_delete;

	AABB aabb = AABB(float3::zero, float3::zero);
	AABB obb = AABB(float3::zero, float3::zero);
	bool see_bounding_box = false;
};

