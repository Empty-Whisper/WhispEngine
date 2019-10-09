#pragma once
#include "Panel.h"
#include "Globals.h"

class PanelAbout: 
	public Panel
{
public:
	PanelAbout(const bool &start_active = true, const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelAbout();

	void Update() override;

private:
	bool checkbox_mit_license_window = false;

};