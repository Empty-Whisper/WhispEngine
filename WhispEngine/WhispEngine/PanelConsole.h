#pragma once
#include "Panel.h"
#include "Imgui/imgui.h"
class PanelConsole :
	public Panel
{
public:
	PanelConsole(const bool &start_active = true, 
				const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelConsole();

	void Update() override;

	void Log(const char* str);

private:
	ImGuiTextBuffer buffer;
	bool log_new_line = false;
};

