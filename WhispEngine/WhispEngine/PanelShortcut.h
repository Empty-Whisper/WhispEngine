#pragma once
#include "Panel.h"
#include <vector>
class PanelShortcut :
	public Panel
{
public:
	PanelShortcut(const bool &start_active = true, const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelShortcut();

	void Update() override;

private:
	bool to_change = false;
	int  scancode_to_change = -1;
	std::vector<ShortCut*>::iterator it;
};

