#pragma once
#include "Panel.h"
#include "GameObject.h"
#include "Imgui/imgui.h"

class PanelHierarchy :
	public Panel
{
public:
	PanelHierarchy(const bool &start_active = true, 
		const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelHierarchy();

	void Update() override;

private:
	void DrawNode(GameObject* const &obj);

	ImGuiTreeNodeFlags node_flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	ImGuiTreeNodeFlags leaf_flag = ImGuiBackendFlags_::ImGuiBackendFlags_None;
	ImGuiTreeNodeFlags select_flag = ImGuiTreeNodeFlags_Selected;
	ImGuiTreeNodeFlags current_flag = ImGuiBackendFlags_::ImGuiBackendFlags_None;

	std::vector<GameObject*> to_delete; // In order to don't cause crash while iterating the tree, we keep the pointers in a vector to later iterate and delete that objects
};

