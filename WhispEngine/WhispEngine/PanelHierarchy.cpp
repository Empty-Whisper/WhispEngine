#include "PanelHierarchy.h"
#include "Application.h"


PanelHierarchy::PanelHierarchy(const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	:Panel("Hierarchy", start_active, shortcut1, shortcut2, shortcut3)
{
	leaf_flag = node_flag;
	leaf_flag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	current_flag = node_flag;
}


PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::Update()
{
	if (ImGui::Begin("Hierarchy", &active)) {
		GameObject* root = App->object_manager->GetRoot();
		for (auto i = root->children.begin(); i != root->children.end(); i++) {
			DrawNode(*i);
		}
		if (!to_delete.empty()) {
			for (auto del = to_delete.begin(); del != to_delete.end(); del++) {
				App->object_manager->DestroyGameObject(*del);
			}
			to_delete.clear();
		}
	}
	ImGui::End();

}

void PanelHierarchy::DrawNode(GameObject* const &obj) {
	if(obj->IsActive() == false)
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.f));

	current_flag = node_flag;

	if (obj->children.empty())
		current_flag = leaf_flag;
	else
		current_flag = node_flag;

	if (App->object_manager->GetSelected() == obj)
		current_flag |= select_flag;

	bool is_open = ImGui::TreeNodeEx(obj, current_flag, obj->GetName());

	if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
		App->object_manager->SetSelected(obj);
	}

	ImGui::PushID(obj);
	if (ImGui::BeginPopupContextItem("object")) {
		if (ImGui::Button("Delete")) {
			to_delete.push_back(obj);
		}

		ImGui::EndPopup();
	}
	ImGui::PopID();

	if (!obj->children.empty() && is_open) {
		for (auto i = obj->children.begin(); i != obj->children.end(); i++) {
			DrawNode(*i);
		}

		ImGui::TreePop();
	}

	if (obj->IsActive() == false)
		ImGui::PopStyleColor();
}
