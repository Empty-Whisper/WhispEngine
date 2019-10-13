#pragma once
#include "Panel.h"
#include "GameObject.h"
#include "Imgui/imgui.h"

class PanelHierarchy :
	public Panel
{
public:
	PanelHierarchy();
	~PanelHierarchy();

	void Update() override;

private:
	void DrawNode(GameObject* const &obj);

	ImGuiTreeNodeFlags node_flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	ImGuiTreeNodeFlags leaf_flag = ImGuiBackendFlags_::ImGuiBackendFlags_None;
	ImGuiTreeNodeFlags select_flag = ImGuiTreeNodeFlags_Selected;
	ImGuiTreeNodeFlags current_flag = ImGuiBackendFlags_::ImGuiBackendFlags_None;

	GameObject* selected = nullptr;
};

