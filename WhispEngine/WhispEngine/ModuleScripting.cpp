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
	if (luaL_loadfile(L, "Assets/Scripts/test.lua") == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(L, "Model");
		if (table.isTable()) {
			if (table["Update"].isFunction()) {
				luabridge::LuaRef func = table["Update"];
				func();
			}
			else {
				LOG("NO ES FUNCCION");
			}
		}
		else {
			LOG("NOOOOOOOOOOOOOOOOOOOO");
		}
	}
	else {
		LOG("%s", lua_tostring(L, -1));
		
	}
	//update();

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
