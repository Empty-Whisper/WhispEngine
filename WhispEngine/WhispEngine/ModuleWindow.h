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

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;
	Uint32 flags;
	//The surface contained by the window
	SDL_Surface* screen_surface;

	//Screen measures
	uint screen_width;
	uint screen_height;

	//Device mode structure
	DEVMODE ScreenSettings;


};

#endif // __ModuleWindow_H__