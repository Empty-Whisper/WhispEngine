#include "PanelObjects.h"
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
		DrawNode(App->object_manager->GetRoot());
		ImGui::End();
	}
}

void PanelHierarchy::DrawNode(GameObject* const &obj) {
	
	for (auto i = obj->children.begin(); i != obj->children.end(); ++i) {

		if ((*i)->children.empty()) {
			current_flag = leaf_flag;
		}
		else {
			current_flag = node_flag;
		}

		if (selected == *i) {
			current_flag |= select_flag;
		}

		bool open = ImGui::TreeNodeEx((void*)(intptr_t)*i, current_flag, (*i)->GetName());

		if (ImGui::IsItemClicked()) {
			selected = *i;
		}

		if (!(*i)->children.empty() && open)
		{
			DrawNode(*i);
			ImGui::TreePop();
			current_flag = node_flag;
		}
	}
}
