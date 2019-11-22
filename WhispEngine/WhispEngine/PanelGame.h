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

	const ImVec2 GetPanelSize();
	const ImVec2 GetPanelPos();

private:

	ImVec2 panel_size = { 0.f, 0.f };
	ImVec2 panel_pos = { 0.f, 0.f };
};

#endif // !_PANEL_GAME__