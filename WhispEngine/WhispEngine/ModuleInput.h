#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"

#include <vector>

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
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	bool GetKeyDown(int id) const
	{
		return keyboard[id] == KEY_DOWN;
	}

	bool GetKeyUp(int id) const
	{
		return keyboard[id] == KEY_UP;
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	bool GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id] == KEY_DOWN;
	}

	bool GetMouseButtonUp(int id) const
	{
		return mouse_buttons[id] == KEY_UP;
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}
	
	int GetMouseWheel() const
	{
		return mouse_z;
	}

	bool IsAltPressed();

private:
	KEY_STATE* keyboard;
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

};