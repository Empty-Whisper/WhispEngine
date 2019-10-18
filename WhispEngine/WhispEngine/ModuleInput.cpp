#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"

#define MAX_KEYS 300


ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name.assign("Input");

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(nlohmann::json &node)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	UpdateInput();

	update_status ret = PollEvents();

	return ret;
}

update_status ModuleInput::PollEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->size;
			mouse_y = e.motion.y / App->window->size;

			mouse_x_motion = e.motion.xrel / App->window->size;
			mouse_y_motion = e.motion.yrel / App->window->size;
			break;

		case SDL_QUIT:
			return UPDATE_STOP;
			break;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				App->renderer3D->OnResize(e.window.data1, e.window.data2);
			break;
		
		case SDL_DROPFILE:
			char* file = e.drop.file;

			switch (App->file_system->GetFormat(file))
			{
			case FileSystem::Format::JSON:
				break;
			case FileSystem::Format::DDS:
				App->importer->ImportTexture(file);
				break;
			case FileSystem::Format::FBX:
				App->importer->ImportFbx(file);
				break;
			default:
				break;
			}

			SDL_free(file);
			break;
		
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleInput::UpdateInput()
{
	UpdateKeyStates();

	UpdateButtonStates();
}

void ModuleInput::UpdateButtonStates()
{
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->size;
	mouse_y /= App->window->size;
	mouse_z = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
			{
				mouse_buttons[i] = KEY_DOWN;
				GetTextBuffer("Mouse", i, "KEY_DOWN");
				GetTextBuffer("Keybr", i, "KEY_REPEAT");
			}
			else
				mouse_buttons[i] = KEY_REPEAT;

		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
			{
				mouse_buttons[i] = KEY_UP;
				GetTextBuffer("Mouse", i, "KEY_UP");
			}

			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;
}

void ModuleInput::UpdateKeyStates()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
				GetTextBuffer("Keybr", i, "KEY_DOWN");
				GetTextBuffer("Keybr", i, "KEY_REPEAT");

			}
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
				GetTextBuffer("Keybr", i, "KEY_UP");
			}


			else
				keyboard[i] = KEY_IDLE;
		}
	}
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::IsAltPressed()
{
	if (GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT)
		return true;

	return false;
}

void ModuleInput::GetTextBuffer(const std::string &key,const int &key_num, const std::string &key_state)
{
	std::string text = key + ": " + std::to_string(key_num) + " - " + key_state + "\n";
	this->text_buffer.appendf(text.c_str());
	auto_scroll = true;
}

