#pragma once
#include "Module.h"

#include "Globals.h"

struct lua_State;

class ModuleScripting :
	public Module
{
public:
	ModuleScripting();
	~ModuleScripting();

	bool Start() override;
	update_status Update() override;

	void ExecuteScript(const char* file);

	void Register();

	lua_State* GetState() const { return L; }

private:
	lua_State* L = nullptr;

	static void Debug(const char* s) {
		LOG(s);
	}
};
