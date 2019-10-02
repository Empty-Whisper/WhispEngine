#pragma once
#include "SDL/include/SDL_scancode.h"
#include <functional>

class ShortCut
{
public:
	ShortCut();
	ShortCut(const std::function<void()> &func, const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~ShortCut();

	//Each shortcut can have a combination of three keys, the first use Ctrl, Alt or Shift to a correct performance
	void SetShortCut(const SDL_Scancode &key1, const SDL_Scancode &key2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &key3 = SDL_SCANCODE_UNKNOWN);
	bool IsShortCutPressed() const;

	void DoFunc();

private:
	SDL_Scancode shortcut[3] = { SDL_SCANCODE_UNKNOWN };

	std::function<void()> function = nullptr;
};
