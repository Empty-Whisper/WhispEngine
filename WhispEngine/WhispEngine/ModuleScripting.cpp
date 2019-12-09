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
	luaL_dofile(L, "Assets/Scripts/test.lua");
	luabridge::LuaRef start = luabridge::getGlobal(L, "Start");

	start();

	return true;
}

update_status ModuleScripting::Update()
{
	luaL_dofile(L, "Assets/Scripts/test.lua");
	luabridge::LuaRef update = luabridge::getGlobal(L, "Update");

	update();

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
