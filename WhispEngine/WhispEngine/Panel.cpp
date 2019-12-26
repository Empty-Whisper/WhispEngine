#include "Panel.h"
#include "Application.h"

Panel::Panel()
{
}

Panel::Panel(const std::string &name, const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	: name(name), active(start_active)
{
	shortcut = new ShortCut(std::bind(&Panel::ChangeActive, this), name.c_str(), shortcut1, shortcut2, shortcut3);
}


Panel::~Panel()
{
	delete shortcut;
	shortcut = nullptr;
}

bool Panel::IsActive() const
{
	return active;
}

void Panel::ChangeActive()
{
	active = !active;
}
