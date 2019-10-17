#include "PanelScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

PanelScene::PanelScene(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3) 
	: Panel("Configuration", start_active, shortcut1, shortcut2, shortcut3)
{
}

PanelScene::~PanelScene()
{
}

void PanelScene::Update()
{
	if (ImGui::Begin("Scene", &active /*ImGuiWindowFlags_NoCollapse  ImGuiNextWindowDataFlags_HasFocus |*/ /*ImGuiHoveredFlags_RootWindow*/)) //ImGuiHoveredFlags_RootWindow  ImGuiFocusedFlags_RootWindow
	{
		ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)App->renderer3D->render_texture, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));

		if (current_viewport_size.x != panel_size.x || current_viewport_size.y != panel_size.y)
		{
			panel_size = current_viewport_size;

			App->renderer3D->CanResize();
		}
		panel_pos = ImGui::GetWindowPos();
	}
	ImGui::End();
}

const ImVec2 PanelScene::GetPanelSize()
{
	return panel_size;
}

const ImVec2 PanelScene::GetPanelPos()
{
	return panel_pos;
}


