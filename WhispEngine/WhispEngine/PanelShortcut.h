#pragma once
#include "Panel.h"
class PanelShortcut :
	public Panel
{
public:
	PanelShortcut(const bool &start_active = true, const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelShortcut();

	void Update() override;
};
