#include "PanelScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

PanelScene::PanelScene(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3) 
	: Panel("Scene", start_active, shortcut1, shortcut2, shortcut3)
{
}

PanelScene::~PanelScene()
{
}

void PanelScene::Update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0,0) );

	if (ImGui::Begin("#Scene", &active)) 
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 70);
		App->gui->HelpMarker("(?)Info", "Right click        - View around \nWASD + Right click - Move through viewport\nShift              - Speed up\nAlt + Right click  - Zoom\nScroll wheel       - Fast Zoom\nMiddle mouse       - Move X/Y viewport\nF                  - Focus object\nAlt + Left click   - Orbit object");

		ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();
		
		//Check if window is Resized
		if (current_viewport_size.x != panel_size.x || current_viewport_size.y != panel_size.y)
		{
			panel_size = current_viewport_size;

			App->renderer3D->CanResize();


			App->renderer3D->OnResize(panel_size.x, panel_size.y);
			ImGuizmo::SetRect(panel_pos.x, panel_pos.y, panel_size.x, panel_size.y);

		}
		panel_pos = ImGui::GetWindowPos();


		// Guizmo
		ImGuizmo::SetDrawlist();

		//App->renderer3D->OnResize(panel_pos.x, panel_pos.y);
		ImGuizmo::SetRect(panel_pos.x, panel_pos.y, current_viewport_size.x, current_viewport_size.y);

		// Render inside Window
		ImGui::Image((ImTextureID)App->renderer3D->render_texture, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
		//  Check if some obj is selected
		if (App->object_manager->GetSelected() != nullptr)
		{
			ImGuizmo::Enable(true);
			App->object_manager->UpdateGuizmo();
		}
		else
			ImGuizmo::Enable(false);
	}
	ImGui::End();
	ImGui::PopStyleVar();

}

const ImVec2 PanelScene::GetPanelSize()
{
	return panel_size;
}

const ImVec2 PanelScene::GetPanelPos()
{
	return panel_pos;
}


