#pragma once
#include "Module.h"

extern "C" {
#include "Lua/Lua/include/lua.h"
#include "Lua/Lua/include/lua.hpp"
#include "Lua/Lua/include/lauxlib.h"
}

#include "Globals.h"

class ModuleScripting :
	public Module
{
public:
	ModuleScripting();
	~ModuleScripting();

	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;

	void ExecuteFunctionScript(const char* path, const char* name, const char* function);
	void ExecuteScript(const char* file);

	void LuaRegister() override;

	lua_State* GetState() const { return L; }

private:
	lua_State* L = nullptr;

	static void Log(const char* s) {
		if (s != nullptr) {
			LOG(s);
		}
		else
			LOG("LUA FAILED: Message is nullptr");
	}
};
