#include "PanelInspector.h"
#include "Application.h"
#include "Imgui/imgui.h"

#include "ModuleSceneIntro.h"
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

			ImGui::SameLine();
			bool to_static = sel->IsStatic();
			static bool has_child_static = false;
			static bool has_parent_dynamic = false;
			if (ImGui::Checkbox("Static", &to_static)) {
				sel->SetStatic(to_static);
				if (to_static) {
					std::vector<GameObject*> parent;
					if (sel->HasDynamicParent(parent)) {
						has_parent_dynamic = true;
						ImGui::OpenPopup("Change Children");
					}
					else {
						if (sel->children.empty()) {
							sel->SetStatic(to_static);
							App->scene_intro->octree->Insert(sel);
						}
						else {
							ImGui::OpenPopup("Change Children");
						}
					}
				}
				else {
					if (sel->children.empty()) {
						sel->SetStatic(to_static);
						App->scene_intro->octree->Remove(sel);
					}
					else {
						if (sel->HasAnyStaticChild())
							has_child_static = true;
						ImGui::OpenPopup("Change Children");
					}
				}
			}
			if (ImGui::BeginPopupModal("Change Children", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				if (!has_child_static && !has_parent_dynamic)
					ImGui::Text("Do you want to change children to %s?\n", to_static ? "static" : "dynamic");
				else if (has_parent_dynamic)
					ImGui::Text("GameObject %s has as minimum one parent dynamic. Dynamic GameObjects cannot\nhave static child. This action will change all children from to dynamic objects.\nDo you want to continue?\n", sel->GetName());
				else if(has_child_static)
					ImGui::Text("GameObject %s has as minimum one child static. Dynamic GameObjects cannot\nhave static child. This action will change all children to dynamic objects.\nDo you want to continue?\n", sel->GetName());
				ImGui::Separator();
				
				if (ImGui::Button(has_parent_dynamic ? "Yes, change parent and children" : "Yes, change children")) {
					if (to_static)
						App->scene_intro->octree->Insert(sel);
					else
						App->scene_intro->octree->Remove(sel);

					sel->SetStatic(to_static);

					std::vector<GameObject*> objects;
					if (has_parent_dynamic)
						sel->HasDynamicParent(objects);
					App->object_manager->GetChildsFrom(sel, objects);

					for (auto i = objects.begin(); i != objects.end(); ++i) {
						if ((*i)->IsStatic() != to_static) {
							(*i)->SetStatic(to_static);
							if (to_static)
								App->scene_intro->octree->Insert(*i);
							else
								App->scene_intro->octree->Remove(*i);
						}
					}
					ImGui::CloseCurrentPopup();
					has_child_static = false;
					has_parent_dynamic = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("No")) {
					if (!has_child_static && !has_parent_dynamic) {
						sel->SetStatic(to_static);
						if (to_static)
							App->scene_intro->octree->Insert(sel);
						else
							App->scene_intro->octree->Remove(sel);
					}
					else
						sel->SetStatic(!to_static);

					ImGui::CloseCurrentPopup();
					has_child_static = false;
					has_parent_dynamic = false;
				}
				ImGui::EndPopup();
			}
			//ImGui::SameLine(); App->gui->HelpMarker("(?)", "Right Click on component header to Delete (only in component not obligatory)");

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
