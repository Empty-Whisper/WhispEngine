#pragma once
#include "Panel.h"
class PanelResources :
	public Panel
{
public:
	PanelResources(const bool & start_active = true, 
		const SDL_Scancode & shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode & shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode & shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelResources();

	void Update();

	std::string file_dragdrop;
private:
	void DrawNode(const char* path);
};

