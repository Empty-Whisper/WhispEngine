#include "PanelConsole.h"
#include "Application.h"



PanelConsole::PanelConsole(const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	:Panel("Console", start_active, shortcut1, shortcut2, shortcut3)
{
	Log(App->GetLog());
}


PanelConsole::~PanelConsole()
{
}

void PanelConsole::Update()
{
	ImGui::Begin(name.data(), &active);
	ImGui::TextUnformatted(buffer.begin());
	if (log_new_line)
		ImGui::SetScrollHereY(1.0f);
	log_new_line = false;
	ImGui::End();
}

void PanelConsole::Log(const char * str)
{
	buffer.appendf(str);
	log_new_line = true;
}
