#pragma once
#include <string>
#include <vector>

#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

#include "JSON/json.hpp"

enum class ObjectSelected
{
	NONE = -1,
	SELECTED, // Orange outline
	CHILD_FROM_PARENT_SELECTED, // Blue outline
};

class GameObject
{
	friend class ComponentMesh;
	friend class ModuleObjectManager;
public:
	GameObject(GameObject *parent);
	~GameObject();

public:
	void Update();

	void DrawBoundingBoxAABB();
	void DrawBoundingBoxOBB();
	AABB GetAABB() const;

	Component* CreateComponent(const ComponentType &type);
	void	   DeleteComponent(Component* comp);
	Component* GetComponent(const ComponentType &type) const;
	bool	   TryGetComponent(const ComponentType &type, Component* &comp) const;
	bool	   HasComponent(const ComponentType &type);

	void Raycast(const LineSegment& ray_cast, bool& intersect, float&  length);

	bool IsActive() const;
	void SetActive(const bool &to_active);

	bool IsStatic() const;
	void SetStatic(bool to_static);

	const char* GetName() const;
	void SetName(const char* name);

	ObjectSelected GetSelect() const;
	void Select();
	void Deselect();

	void Detach();
	void Attach(GameObject* parent);
	bool HasChild(GameObject* child) const;
	bool HasAnyStaticChild() const;
	bool HasDynamicParent(std::vector<GameObject*>& parents) const;

	bool Save(nlohmann::json &node);

public:
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	bool is_inside_frustum = false;

private:
	bool active = true;
	bool obj_static = false;
	std::string name;
	ObjectSelected obj_selected = ObjectSelected::NONE;
	std::vector<Component*> components_to_delete;

	uint64_t UID = 0u;

	bool see_bounding_box = false; // TODO: Set in component mesh ?
};

