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
				if (!(*chng).second->HasChild((*chng).first)) { // if the GameObject we want to change is parent of the other, we will leap that situation
					(*chng).second->Detach();
					(*chng).second->Attach((*chng).first);
				}
			}
			to_change.clear();
		}
		if (!to_move.empty()) {
			for (auto it = to_move.begin(); it != to_move.end(); it++) {
				switch ((*it).direction) {
				case ToMove::UP:
					std::iter_swap((*it).object, (*it).object - 1);
					break;
				case ToMove::DOWN:
					std::iter_swap((*it).object, (*it).object + 1);
					break;
				}
			}
			to_move.clear();
		}
		if (to_create != nullptr) {
			App->object_manager->CreateGameObject(to_create);
			to_create = nullptr;
		}

		if(App->input->GetMouseButtonDown(0))
			popup_window = false;
		if(!ImGui::IsAnyItemHovered() || popup_window)
			if (ImGui::BeginPopupContextWindow("HierarchyPopup",1,false)) {
				popup_window = true;
				if (ImGui::Selectable("Create Empty")) {
					App->object_manager->CreateGameObject(nullptr);
					ImGui::CloseCurrentPopup();
					popup_window = false;
				}
				ImGui::Separator();
				if (ImGui::BeginMenu("3D Object")) {
					/*if (ImGui::Selectable("Cube")) // TODO: Do a for loop or a ImGui::Combo
						App->object_manager->CreatePrimitive(Primitives::CUBE, Object_data());
					if (ImGui::Selectable("Tetrahedron"))
						App->object_manager->CreatePrimitive(Primitives::TETRAHEDRON, Object_data());
					if (ImGui::Selectable("Octahedron"))
						App->object_manager->CreatePrimitive(Primitives::OCTAHEDRON, Object_data());
					if (ImGui::Selectable("Dodecahedron"))
						App->object_manager->CreatePrimitive(Primitives::DODECAHEDRON, Object_data());
					if (ImGui::Selectable("Icosahedron"))
						App->object_manager->CreatePrimitive(Primitives::ICOSAHEDRON, Object_data());
					if (ImGui::Selectable("Sphere"))
						App->object_manager->CreatePrimitive(Primitives::SPHERE, Object_data());
					if (ImGui::Selectable("Hemisphere"))
						App->object_manager->CreatePrimitive(Primitives::HEMISPHERE, Object_data());
					if (ImGui::Selectable("Torus"))
						App->object_manager->CreatePrimitive(Primitives::TORUS, Object_data());
					if (ImGui::Selectable("Cone"))
						App->object_manager->CreatePrimitive(Primitives::CONE, Object_data());
					if (ImGui::Selectable("Cylinder"))
						App->object_manager->CreatePrimitive(Primitives::CYLINDER, Object_data());*/
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
		if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), false)) {
			if (App->input->GetMouseButtonDown(1))
				App->object_manager->SetSelected(nullptr);
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

		if (ImGui::Button("Create Empty")) {
			to_create = obj;
			ImGui::CloseCurrentPopup();
		}

		ToMove tmp;
		tmp.object = std::find(obj->parent->children.begin(), obj->parent->children.end(), obj);
		bool refuse_move = tmp.object == obj->parent->children.begin() || obj->parent->children.empty();
		
		if (refuse_move) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		
		if (ImGui::Button("Move Up")) {
			tmp.direction = tmp.UP;
			to_move.push_back(tmp);
			ImGui::CloseCurrentPopup();
		}
		
		if (refuse_move) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		refuse_move = tmp.object == obj->parent->children.end() - 1 || obj->parent->children.empty();

		if (refuse_move) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Button("Move Down")) {
			tmp.direction = tmp.DOWN;
			to_move.push_back(tmp);
			ImGui::CloseCurrentPopup();
		}

		if (refuse_move) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}


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
			to_change.emplace(std::pair<GameObject*, GameObject*>(obj, *(GameObject**)payload->Data));
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), ImGui::GetID("Hierarchy"))) { //Window
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_POINTER")) {
			to_change.emplace(std::pair<GameObject*, GameObject*>(App->object_manager->GetRoot(), *(GameObject**)payload->Data));
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
