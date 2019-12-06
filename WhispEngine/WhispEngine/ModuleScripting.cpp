extern "C"{
#include "Lua/Lua/include/lua.h"
#include "Lua/Lua/include/lua.hpp"
#include "Lua/Lua/include/lauxlib.h"
}

#pragma comment(lib, "Lua/Lua/liblua53.a")

#include "ModuleScripting.h"

#include "Lua/LuaBridge/LuaBridge.h"

ModuleScripting::ModuleScripting()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	Register();
}


ModuleScripting::~ModuleScripting()
{
	lua_close(L);
}

bool ModuleScripting::Start()
{
	ExecuteScript("Assets/Scripts/test.lua");
	return true;
}

update_status ModuleScripting::Update()
{
	ExecuteScript("Assets/Scripts/test.lua");
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

void Debug(const char* s) {
	LOG(s);
}

void ModuleScripting::Register()
{
	using namespace luabridge;
	getGlobalNamespace(L)
		.beginNamespace("whisp")
			.addFunction("Debug", &ModuleScripting::Debug)
		.endNamespace();
}
