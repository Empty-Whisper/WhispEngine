#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"

#include <vector>
#include <map>

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init(nlohmann::json &node);
	update_status PreUpdate();
	update_status PollEvents();
	void UpdateInput();
	void UpdateButtonStates();
	void UpdateKeyStates();

	void LuaRegister() override;

	//Always use the left key
	void AllowLeftAndRightKeys(const SDL_Scancode key);
	bool CleanUp();

	KEY_STATE GetKey(int id) const;
	static bool LGetKey(const char* id);

	bool GetKeyDown(int id) const;
	static bool LGetKeyDown(const char* id);
	bool GetKeyUp(int id) const;
	static bool LGetKeyUp(const char* id);

	KEY_STATE GetMouseButton(int id) const;
	bool GetMouseButtonDown(int id) const;
	bool GetMouseButtonUp(int id) const;

	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseZ() const;

	int GetMouseXMotion() const;
	int GetMouseYMotion() const;
	
	int GetMouseWheel() const;

private:
	static KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;

public:
	ImGuiTextBuffer text_buffer;
	void GetTextBuffer(const std::string &key, const int &key_num, const std::string &key_state);
	bool auto_scroll = false;

	//lua
private:
	static std::map<std::string, SDL_Scancode> lua_map;
};