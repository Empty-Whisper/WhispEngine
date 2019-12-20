#pragma once
#include "Component.h"

class ComponentScript :
	public Component
{
	enum class TypeData {
		NONE = -1,
		BOOL, INT, FLOAT, NIL, TABLE, STRING, USERDATA,
		MAX
	};
	class PublicVar {
	public:
		PublicVar(TypeData type) : type(type) {}
		TypeData type = TypeData::NONE; //TODO To enum
	};
	template<typename T>
	class Property : public PublicVar {
	public:
		Property(TypeData type, const T& data) : PublicVar(type), data(data) {}
		T data;
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

private:
	void SetScriptName();
	void SetInspectorVars();
	void DrawInspectorVars();
	void OpenModalWindowsToLoadScript();
	void UpdateInspectorVars();

private:
	std::map<std::string, PublicVar*> public_vars;

	bool first_time = true;
};
