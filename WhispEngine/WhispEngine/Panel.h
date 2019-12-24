#pragma once
#include <string>
#include "ShortCut.h"

class Panel
{
public:
	Panel();
	Panel(const std::string &name, const bool &start_active = true, 
			const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	virtual ~Panel();

	bool IsActive() const;
	void ChangeActive();

	virtual void Update() = 0;

public:
	std::string name;
	bool active = true;

	//ImVec2 rect[2];
	ShortCut *shortcut = nullptr;
};
