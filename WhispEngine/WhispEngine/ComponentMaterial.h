#pragma once
#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();
};

