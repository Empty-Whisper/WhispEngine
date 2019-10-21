#pragma once
#include "Panel.h"
#include <vector>
#include "ModuleObjectManager.h"

class PanelCreate :
	public Panel
{
public:
	PanelCreate(const bool &start_active = true,
		const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelCreate();

	void Update() override;

private:
	std::vector<std::string> items;
	std::vector<std::string>::iterator iterator;

	Object_data data;
};

