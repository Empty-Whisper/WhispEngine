#include "PanelInspector.h"
#include "Application.h"
#include "Imgui/imgui.h"

#include "ModuleGUI.h"
#include "ModuleObjectManager.h"

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

			bool to_active = sel->IsActive();
			if (ImGui::Checkbox("##Active", &to_active))
				sel->SetActive(to_active);
			ImGui::SameLine();

			char name[100];
			sprintf_s(name, 100, sel->GetName());
			if (ImGui::InputText("##Name", name, 100)) {
				sel->SetName(name);
			}

			ImGui::SameLine(); App->gui->HelpMarker("(?)", "Right Click on component header to Delete (only not obligatory)");

			for (auto i = sel->components.begin(); i != sel->components.end(); i++) {
				ImGui::PushID(*i);
				(*i)->OnInspector();
				ImGui::Separator();
				ImGui::PopID();
			}
			ImGui::NewLine();

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("components_popup");
			if (ImGui::BeginPopup("components_popup")) {
				if (ImGui::Selectable("Mesh")) // TODO: Do a for loop or a ImGui::Combo
					if (!sel->HasComponent(ComponentType::MESH))
						sel->CreateComponent(ComponentType::MESH);
				if(ImGui::Selectable("Camera"))
					if (!sel->HasComponent(ComponentType::CAMERA))
						sel->CreateComponent(ComponentType::CAMERA);

				ImGui::EndPopup();
			}
		}

	}
	ImGui::End();

}
