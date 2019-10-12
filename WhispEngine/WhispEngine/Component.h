#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

#pragma once

enum ComponentType
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
	LIGHT
};


class GameObject;

class Component
{
public:
	Component(GameObject* parent, ComponentType type = ComponentType::NONE);
	virtual ~Component();

public:

	virtual void Update();
	virtual void Init();

	virtual void Enable();
	virtual void Disable();

	const bool IsActive();
	const ComponentType GetType();


public:
	GameObject* parent;

private:
	ComponentType type;
	bool is_active;
};
#endif
