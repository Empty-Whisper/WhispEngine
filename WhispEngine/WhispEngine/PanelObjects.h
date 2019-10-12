#pragma once
#include "Panel.h"
#include "ComponentMesh.h"
#include "Imgui/imgui.h"

class PanelHierarchy :
	public Panel
{
public:
	PanelHierarchy();
	~PanelHierarchy();

	void Update() override;

	const char* normal_name[(int)Normals::MAX] = { "No view","Faces","Vertex" };

private:
	void DrawNode(GameObject* &obj);

	ImGuiTreeNodeFlags node_flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	ImGuiTreeNodeFlags leaf_flag = ImGuiBackendFlags_::ImGuiBackendFlags_None;
	ImGuiTreeNodeFlags current_flag = ImGuiBackendFlags_::ImGuiBackendFlags_None;
};

