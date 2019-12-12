#pragma once
#include "Component.h"

class ComponentScript :
	public Component
{
public:
	ComponentScript(GameObject* parent);
	~ComponentScript();

	void Update() override;

	void OnInspector();

	void Save(nlohmann::json &node);
	void Load(const nlohmann::json &node);

	std::string script_path = SCRIPT_A_FOLDER;
	std::string name = "NONE";
	std::string title = "NONE (Script)";
	bool is_assigned = false;	
};

