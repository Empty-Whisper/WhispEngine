#pragma once
#include "Panel.h"

class GameObject;

class PanelInspector :
	public Panel
{
public:
	PanelInspector(const bool &start_active = true, 
		const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelInspector();

public:
	void Update();
	void StaticLogic(GameObject * &sel);
};

