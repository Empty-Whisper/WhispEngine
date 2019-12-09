#pragma once
#include "Component.h"
class ComponentScript :
	public Component
{
public:
	ComponentScript(GameObject* parent);
	~ComponentScript();

	void Update() override {};

	void OnInspector();

	std::string script_path = SCRIPT_A_FOLDER;
	std::string name = "NONE";
	std::string title = "NONE (Script)";
	bool is_assigned = false;
};

