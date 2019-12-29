#include "PanelShortcut.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "ModuleShortCut.h"
#include "ModuleInput.h"
#include "mmgr/mmgr.h"
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
			ImGui::PushID(*i);
			ImGui::Text(((*i)->name + ": ").c_str());
			for (int j = 0; j < 3; j++) {
				ImGui::PushID(j);
				ImGui::SameLine();
				if (ImGui::Button(SDL_GetScancodeName((*i)->shortcut[j]), ImVec2(100,30))) {
					to_change = true;
					it = i;
					scancode_to_change = j;
				}
				ImGui::PopID();
			}
			ImGui::PopID();
		}
		if (to_change) {
			auto scancode = App->input->GetFirstKey();
			if (scancode != SDL_SCANCODE_UNKNOWN) {
				if (scancode == SDL_SCANCODE_ESCAPE)
					scancode = SDL_SCANCODE_UNKNOWN;
				(*it)->shortcut[scancode_to_change] = scancode;
				to_change = false;
				it = App->shortcut->shortcuts.end();
				scancode_to_change = -1;
			}
		}
	}
	ImGui::End();
}
