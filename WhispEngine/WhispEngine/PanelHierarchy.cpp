#include "PanelHierarchy.h"
#include "Application.h"
#include "Imgui/imgui_internal.h"


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
		if (!to_change.empty()) {
			for (auto chng = to_change.begin(); chng != to_change.end(); chng++) {
				if ((*chng).child->HasChild((*chng).parent)) { // if we change the parent-child to child-parent, causes interferences
					//(*chng).parent->Detach();					//so first detach the parent and set to the childs parent
					//(*chng).parent->Attach((*chng).child->parent);
					//(*chng).child->Detach();					//now attach the child to the new parent in the new position
					//(*chng).child->Attach((*chng).parent);
				}
				else {
					(*chng).child->Detach();
					(*chng).child->Attach((*chng).parent);
				}
			}
			to_change.clear();
		}
	}
	ImGui::End();

}

void PanelHierarchy::DrawNode(GameObject* obj) {
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

	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("CHILD_POINTER", &obj, sizeof(int));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_POINTER")) { // Other GameObject
			ToChange tmp;
			tmp.parent = obj;
			tmp.child = *(GameObject**)payload->Data;
			to_change.push_back(tmp);
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), ImGui::GetID("Hierarchy"))) { //Window
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_POINTER")) {
			ToChange tmp;
			tmp.parent = App->object_manager->GetRoot();
			tmp.child = *(GameObject**)payload->Data;
			to_change.push_back(tmp);
		}
		ImGui::EndDragDropTarget();
	}
	
	if (!obj->children.empty() && is_open) {
		for (auto i = obj->children.begin(); i != obj->children.end(); i++) {
			DrawNode(*i);
		}

		ImGui::TreePop();
	}

	if (obj->IsActive() == false)
		ImGui::PopStyleColor();
}
