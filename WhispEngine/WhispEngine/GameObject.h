#pragma once
#include <string>
#include <vector>

#include "Component.h"

enum class ObjectSelected
{
	NONE = -1,
	SELECTED, // Orange outline
	CHILD_FROM_PARENT_SELECTED, // Blue outline
};

class GameObject
{
public:
	GameObject(GameObject *parent);
	~GameObject();

public:
	void Update();
	Component* CreateComponent(const ComponentType &type);
	Component* GetComponent(const ComponentType &type);

	bool IsActive() const;
	void SetActive(const bool &to_active);

	const char* GetName() const;
	void SetName(const char* name);

	ObjectSelected GetSelect() const;
	void Select();
	void Deselect();

public:
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	GameObject* parent = nullptr;

private:
	bool active = true;
	std::string name;
	ObjectSelected obj_selected = ObjectSelected::NONE;

};

