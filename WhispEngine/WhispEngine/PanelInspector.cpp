#include "PanelInspector.h"
#include "Application.h"
#include "Imgui/imgui.h"

PanelInspector::PanelInspector(const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	:Panel("Inspector", start_active, shortcut1, shortcut2, shortcut3)
{
}


PanelInspector::~PanelInspector()
{
}

void PanelInspector::Update()
{
	if (ImGui::Begin("Inspector", &active)) {
		GameObject* sel = App->object_manager->GetSelected();
		if (sel != nullptr) {

			ImGui::PushID(sel);
			bool to_active = sel->IsActive();
			if (ImGui::Checkbox("", &to_active))
				sel->SetActive(to_active);
			ImGui::PopID();
			ImGui::SameLine();

			ImGui::PushID("GameObject_name");
			char a[100];
			sprintf_s(a, sel->GetName());
			if (ImGui::InputText("", a, 32)) {
				sel->SetName(a);
			}
			ImGui::PopID();

			for (auto i = sel->components.begin(); i != sel->components.end(); i++) {
				ImGui::PushID(*i);
				(*i)->OnInspector();
				ImGui::PopID();
			}
		}

	}
	ImGui::End();

}
