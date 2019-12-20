#pragma comment(lib, "Lua/Lua/liblua53.a")

#include "ModuleScripting.h"

#include "Lua/LuaBridge/LuaBridge.h"

#include "Application.h"

ModuleScripting::ModuleScripting()
{
	L = luaL_newstate();
	luaL_openlibs(L);
}


ModuleScripting::~ModuleScripting()
{
	lua_close(L);
}

bool ModuleScripting::Start()
{
	luaL_loadfile(L, "Assets/Internal/GameObject.lua");
	return true;
}

update_status ModuleScripting::PreUpdate()
{
	// TODO: preupdate, update and postupdate all scripts

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleScripting::Update()
{
	//ExecuteFunctionScript("Assets/Scripts/test.lua", "Test", "Update");
	return update_status::UPDATE_CONTINUE;
}

void ModuleScripting::ExecuteFunctionScript(const char* path, const char* _name, const char* function, bool first)
{
	if ((first) ? luaL_dofile(L, path) == 0 : luaL_loadfile(L, path) == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(L, _name);
		if (table.isTable()) {
			if (table[function].isFunction()) {
				luabridge::LuaRef func = table[function];
				func();
			}
			else {
				LOG("%s is not a function or was not found", function);
			}
		}
		else {
			LOG("Cannot find table %s", _name);
		}
	}
	else {
		LOG("Lua Error: %s", lua_tostring(L, -1));
	}
}

update_status ModuleScripting::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

void ModuleScripting::ExecuteScript(const char * file)
{
	if (file == nullptr)
		return;

	int result = luaL_dofile(L, file);

	if (result != 0)
		LOG("LUA ERROR: %s", lua_tostring(L, result));
}

void ModuleScripting::LuaRegister()
{
	using namespace luabridge;
	getGlobalNamespace(L)
		.addFunction("Log", &ModuleScripting::Log);
}
