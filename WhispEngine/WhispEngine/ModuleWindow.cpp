#include "Globals.h"
#include "ModuleWindow.h"
#include "Application.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	name.assign("Window");

	window = NULL;
	screen_surface = NULL;

}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(nlohmann::json &node)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		screen_width = node["width"];
		screen_height = node["height"];
		bright = node["bright"];
		size = node["size"];

		//Create window
		int width = screen_width * size;
		int height = screen_height * size;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


		if(node["flags"]["fullscreen"])
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(node["flags"]["resizable"])
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(node["flags"]["borderless"])
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(node["flags"]["fullscreen_desktop"])
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetAppName(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::Save(nlohmann::json & node) const
{
	node["width"] = screen_width;
	node["height"] = screen_height;
	node["bright"] = bright;

	return true;
}

bool ModuleWindow::Load(nlohmann::json & node)
{
	size = node["size"];

	screen_width = node["width"];
	screen_height = node["height"];
	
	bright = node["bright"];

	SetTitle(App->GetAppName());
	SetWindowSize(screen_width, screen_height);
	SDL_SetWindowBrightness(window, bright);

	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWindowSize(const uint & width, const uint & height)
{
	SDL_SetWindowSize(window, width, height);
}
