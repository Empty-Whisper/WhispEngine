#pragma once
#include "Component.h"
#include "ModuleScripting.h"
#include "Lua/LuaBridge/LuaBridge.h"

class ComponentScript :
	public Component
{
	enum class TypeData {
		NONE = -1,
		BOOL, INT, FLOAT, NIL, TABLE, STRING, USERDATA,
		GAMEOBJECT, PREFAB,
		MAX
	};
	enum class Macros {
		NONE = -1,
		ISGAMEOBJECT,
		DRAG, INPUT, SLIDER,
		MAX
	};
	class PublicVar {
	public:
		PublicVar(TypeData type) : type(type) {}
		virtual ~PublicVar() {}
		TypeData type = TypeData::NONE;
		std::list<Macros> macros;
		std::map<std::string, float> macros_info;
	};
	template<typename T>
	class Property : public PublicVar {
	public:
		Property(TypeData type, const T& data) : PublicVar(type), data(data) {}
		~Property() {}
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
	void GetInspectorVars();
	void DrawInspectorVars();
	void OpenModalWindowsToLoadScript();
	bool UpdateInspectorVars();

private:
	std::map<std::string, PublicVar*> public_vars;

	luabridge::LuaRef script;

	bool start_done = false;
};
