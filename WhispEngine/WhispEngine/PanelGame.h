#pragma once
#ifndef _PANEL_GAME__
#define _PANEL_GAME__

#include "Panel.h"
#include "Globals.h"
#include "Imgui/imgui.h"

class PanelGame : public Panel
{
public:
	PanelGame(const bool &start_active = true,
		const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelGame();

	void Update() override;

	static const ImVec2 GetPanelSize();
	const ImVec2 GetPanelPos();

	bool focused = false;

public:

	static ImVec2 panel_size;
	ImVec2 panel_pos;
};

#endif // !_PANEL_GAME__