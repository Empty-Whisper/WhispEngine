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
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleScripting::Update()
{
	if (first_frame)
		to_change = true;
	return update_status::UPDATE_CONTINUE;
}

void ModuleScripting::ExecuteFunctionScript(const char* path, const char* _name, Functions function)
{
	int result = -1;
	std::string func_name;
	switch (function)
	{
	case ModuleScripting::START:
		result = luaL_dofile(L, path);
		func_name.assign("Start");
		break;
	case ModuleScripting::PREUPDATE:
		func_name.assign("PreUpdate");
		result = luaL_loadfile(L, path);
		break;
	case ModuleScripting::UPDATE:
		func_name.assign("Update");
		result = luaL_loadfile(L, path);
		break;
	case ModuleScripting::POSTUPDATE:
		func_name.assign("PostUpdate");
		result = luaL_loadfile(L, path);
		break;
	default:
		LOG("Function not found")
		break;
	}
	if (result == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(L, _name);
		if (table.isTable()) {
			if (table[func_name.c_str()].isFunction()) {
				luabridge::LuaRef func = table[func_name.c_str()];
				func();
			}
			else {
				LOG("%s is not a function or was not found", func_name.c_str());
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
	if (to_change) {
		first_frame = false;
		to_change = false;
	}
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
