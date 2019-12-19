#pragma once
#include "Component.h"

class ComponentScript :
	public Component
{
	struct PublicVar {
		float value = 0.f;
		std::string type; //TODO To enum
	};
public:
	ComponentScript(GameObject* parent);
	~ComponentScript();

	void Update() override;

	void OnInspector();

	void SetScript(const char* path);

	void Save(nlohmann::json &node);
	void Load(const nlohmann::json &node);

	std::string script_path = SCRIPT_A_FOLDER;
	std::string name = "NONE";
	std::string title = "NONE (Script)";
	bool is_assigned = false;
	bool valid = false;

	std::map<std::string, PublicVar> public_vars;
};

