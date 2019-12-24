#include "PanelShortcut.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "ModuleShortCut.h"

PanelShortcut::PanelShortcut(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3)
	: Panel("ShortCut", start_active, shortcut1, shortcut2, shortcut3)
{
}

PanelShortcut::~PanelShortcut()
{
}

void PanelShortcut::Update()
{
	if (ImGui::Begin("SortCut Editor", &active))
	{
		for (auto i = App->shortcut->shortcuts.begin(); i != App->shortcut->shortcuts.end(); i++) {
			ImGui::Text(((*i)->name + ": ").c_str());
			//ImGui::Button((*i)->shortcut)
		}
	}
	ImGui::End();
}
