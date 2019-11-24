#include "PanelScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentCamera.h"
#include "PanelInspector.h"
#include "ModuleRenderer3D.h"
#include "ModuleObjectManager.h"

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

	if (ImGui::Begin("Scene", &active)) 
	{
	
		ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();
		
		static bool first_time_resize = true;
		//Check if window is Resized
		if ((current_viewport_size.x != panel_size.x || current_viewport_size.y != panel_size.y) || first_time_resize)
		{
			panel_size = current_viewport_size;

			App->renderer3D->CanResize(App->renderer3D->GetSceneViewport());
			App->renderer3D->OnResize(panel_size.x, panel_size.y, App->camera->GetSceneCamera());

			ImGuizmo::SetRect(panel_pos.x, panel_pos.y, panel_size.x, panel_size.y);

			first_time_resize = false;
		}
		panel_pos = ImGui::GetWindowPos();


		// Guizmo
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(panel_pos.x, panel_pos.y, current_viewport_size.x, current_viewport_size.y);

		// Render inside Window
		ImGui::Image((ImTextureID)App->renderer3D->scene_viewport->render_texture, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
		
		//  Check if some obj is selected
		if (App->object_manager->GetSelected() != nullptr)
		{
			ImGuizmo::Enable(true);
			App->object_manager->UpdateGuizmo();

			if (((ComponentCamera*)(App->object_manager->GetSelected())->GetComponent(ComponentType::CAMERA)) != nullptr)
				active_preview = true;
			else
				active_preview = false;
		}
		else
		{
			ImGuizmo::Enable(false);
			active_preview = false;
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();

	if (active_preview && preview_checkbox)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin("Camera Preview", &active, ImGuiWindowFlags_NoDocking))
		{
			static bool init_window = true;
			if (init_window)
			{
				ImVec2 preview_size = ImVec2(225, 225);
				ImGui::SetWindowPos(ImVec2(panel_pos.x + panel_size.x - preview_size.x - 20, panel_pos.y + panel_size.y - preview_size.y));
				ImGui::SetWindowSize(preview_size);
				init_window = false;
			}
			
			ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();
			ImGui::Image((ImTextureID)App->renderer3D->game_viewport->render_texture, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));

		}
		ImGui::End();
		ImGui::PopStyleVar();

	}
	
}

const ImVec2 PanelScene::GetPanelSize()
{
	return panel_size;
}

const ImVec2 PanelScene::GetPanelPos()
{
	return panel_pos;
}


