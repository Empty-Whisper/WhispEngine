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
		for (auto i = App->object_manager->GetRoot()->children.begin(); i != App->object_manager->GetRoot()->children.end(); i++) {
			DrawNode(*i);
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

	ImGui::PushID(obj);
	if (ImGui::BeginPopupContextItem("I am a popu")) {
		ImGui::Button("Button");

		ImGui::EndPopup();
	}
	ImGui::PopID();

	if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
		App->object_manager->SetSelected(obj);
	}

	if (!obj->children.empty() && is_open) {
		for (auto i = obj->children.begin(); i != obj->children.end(); i++) {
			DrawNode(*i);
		}

		ImGui::TreePop();
	}

}
