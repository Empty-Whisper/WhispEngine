#pragma once
#include <string>
#include <vector>

#include "Component.h"

class GameObject
{
public:
	GameObject(GameObject *parent);
	~GameObject();

public:
	void Update();
	Component* CreateComponent(const ComponentType &type);

	bool IsActive() const;
	void SetActive(const bool &to_active);

	const char* GetName() const;
	void SetName(const char* name);

public:
	std::vector<GameObject*> children;

private:
	bool active = true;

	std::string name;
	std::vector<Component*> components;

	GameObject* parent = nullptr;
	
};

