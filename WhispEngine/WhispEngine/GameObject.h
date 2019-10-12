#pragma once
#include <string>
#include <vector>

#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

public:
	void Update();
	Component* CreateComponent(const ComponentType &type);

	bool IsActive() const;
	void SetActive(const bool &to_active);

public:
	std::vector<GameObject*> children;

private:
	bool active = true;

	std::string name;
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	
};

