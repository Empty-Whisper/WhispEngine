#pragma once
#include <string>
#include "SDL/include/SDL_scancode.h"

class Panel
{
public:
	Panel();
	Panel(const std::string &name, const bool &start_active = true, 
			const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	virtual ~Panel();

	bool IsActive() const;
	void ChangeActive();

	//Each panel can have a combination of three keys, the first use Ctrl, Alt or Shift to a correct performance
	void SetShortCut(const SDL_Scancode &key1, const SDL_Scancode &key2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &key3 = SDL_SCANCODE_UNKNOWN);
	bool IsShortCutPressed() const;

	virtual void Update() = 0;

public:
	std::string name;
	bool active = true;

private:
	SDL_Scancode shortcut[3] = {SDL_SCANCODE_UNKNOWN};
};

