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
		GameObject* root = App->object_manager->GetRoot();
		for (auto i = root->children.begin(); i != root->children.end(); ++i) {
			if(ImGui::TreeNodeEx((void*)(intptr_t)*i, node_flag, (*i)->GetName())) {
				DrawNode(*i);

				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}

void PanelHierarchy::DrawNode(GameObject* &obj) {
	for (auto i = obj->children.begin(); i != obj->children.end(); ++i) {

		if ((*i)->children.empty())
			current_flag = leaf_flag;
		else {
			current_flag = node_flag;
		}

		bool open = ImGui::TreeNodeEx((void*)(intptr_t)*i, current_flag, (*i)->GetName());

		if (!(*i)->children.empty() && open)
		{
			DrawNode(*i);
			if (open)
				ImGui::TreePop();
			current_flag = node_flag;
		}
		
	}
}
