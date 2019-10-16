#include "PanelHierarchy.h"
#include "Application.h"


PanelHierarchy::PanelHierarchy()
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

}
