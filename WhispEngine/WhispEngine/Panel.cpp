#include "Panel.h"
#include "Application.h"

Panel::Panel()
{
}

Panel::Panel(const std::string &name, const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	: name(name), active(start_active), shortcut{ shortcut1, shortcut2, shortcut3}
{
}


Panel::~Panel()
{
}

bool Panel::IsActive() const
{
	return active;
}

void Panel::ChangeActive()
{
	active = !active;
}

void Panel::SetShortCut(const SDL_Scancode &key1, const SDL_Scancode & key2, const SDL_Scancode & key3)
{
	shortcut[0] = key1;
	shortcut[1] = key2;
	shortcut[2] = key3;
}

bool Panel::IsShortCutPressed() const
{
	bool ret = false;

	if (shortcut[2] == SDL_SCANCODE_UNKNOWN) {
		if (shortcut[1] == SDL_SCANCODE_UNKNOWN) {
			ret = App->input->GetKeyDown(shortcut[0]);
		}
		else {
			ret = (App->input->GetKey(shortcut[0]) && App->input->GetKeyDown(shortcut[1]));
		}
	}
	else {
		ret = (App->input->GetKey(shortcut[0]) && App->input->GetKey(shortcut[1]) && App->input->GetKeyDown(shortcut[2]));
	}

	return ret;
}
