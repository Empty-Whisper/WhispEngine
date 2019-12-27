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
	if (luaL_dofile(L, "Assets/Internal/Model.lua") == 0) {
		luabridge::LuaRef i = luabridge::getGlobal(L, "Init");
		luabridge::LuaRef table = i();
		if (table.isTable()) {
			LOG("%f", table["speed"].cast<float>());
			table["Update"]();
			table["Update"]();
			table["Update"]();
			LOG("%f", table["speed"].cast<float>());

			luaL_dofile(L, "Assets/Internal/Model.lua");
			luabridge::LuaRef j = luabridge::getGlobal(L, "Init");
			luabridge::LuaRef table2 = j();
			LOG("%f", table["speed"].cast<float>());
			LOG("2: %f", table2["speed"].cast<float>());
			/*if (table["speed"].isFunction()) {
				luabridge::LuaRef func = table[func_name.c_str()];
				func();
			}
			else {
				LOG("%s is not a function or was not found", func_name.c_str());
			}*/
		}
		/*else {
			LOG("Cannot find table %s", _name);
		}*/
	}
	else {
		LOG("Lua Error: %s", lua_tostring(L, -1));
	}
	luabridge::LuaRef f = luabridge::newTable(L);
	auto jaja = f["Update"]();

	return true;
}

update_status ModuleScripting::PreUpdate()
{
	if (luaL_dofile(L, "Assets/Internal/Model.lua") == 0) {
		luabridge::LuaRef i = luabridge::getGlobal(L, "Init");
		luabridge::LuaRef table = i();
		if (table.isTable()) {
			LOG("%f", table["speed"].cast<float>());
			table["Update"]();
			table["Update"]();
			table["Update"]();
			LOG("%f", table["speed"].cast<float>());

			luaL_dofile(L, "Assets/Internal/Model.lua");
			luabridge::LuaRef j = luabridge::getGlobal(L, "Init");
			luabridge::LuaRef table2 = j();
			LOG("%f", table["speed"].cast<float>());
			LOG("2: %f", table2["speed"].cast<float>());
			/*if (table["speed"].isFunction()) {
				luabridge::LuaRef func = table[func_name.c_str()];
				func();
			}
			else {
				LOG("%s is not a function or was not found", func_name.c_str());
			}*/
		}
		/*else {
			LOG("Cannot find table %s", _name);
		}*/
	}
	else {
		LOG("Lua Error: %s", lua_tostring(L, -1));
	}
	luabridge::LuaRef f = luabridge::newTable(L);
	auto jaja = f["Update"]();
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleScripting::Update()
{
	ExecuteFunctionScript("Assets/Internal/Model.lua", "Model", Functions::UPDATE);
	return update_status::UPDATE_CONTINUE;
}

void ModuleScripting::ExecuteFunctionScript(const char* path, const char* _name, Functions function)
{
	int result = -1;
	std::string func_name;
	switch (function)
	{
	case ModuleScripting::START: {
		func_name.assign("Start");
		result = luaL_dofile(L, path);
		luabridge::LuaRef f = luabridge::newTable(L);
	}
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
