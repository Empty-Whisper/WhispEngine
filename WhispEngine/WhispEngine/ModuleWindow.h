#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(nlohmann::json &node);
	bool CleanUp();

	bool Save(nlohmann::json &node) const;
	bool Load(nlohmann::json &node);

	void SetTitle(const char* title);
	void SetWindowSize(const uint &width, const uint &height);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;
	Uint32 flags;
	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	//Screen measures
	uint screen_width = 0;
	uint screen_height = 0;
	uint size = 1;

	float bright = 1.0f;

	//Device mode structure
	DEVMODE ScreenSettings;


};

#endif // __ModuleWindow_H__