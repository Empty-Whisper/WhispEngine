#pragma once
#include "Component.h"
class ComponentScript :
	public Component
{
public:
	ComponentScript(GameObject* parent);
	~ComponentScript();

	void Update() override {};


};

