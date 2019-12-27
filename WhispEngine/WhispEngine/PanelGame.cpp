#include "PanelGame.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "PanelScene.h"
#include "ModuleGUI.h"
#include "ModuleRenderer3D.h"
#include "Brofiler/Brofiler.h"

PanelGame::PanelGame(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3)
	: Panel("Game", start_active, shortcut1, shortcut2, shortcut3)
{
}
PanelGame::~PanelGame()
{
}

void PanelGame::Update()
{
	BROFILER_CATEGORY("Game", Profiler::Color::Moccasin);

	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (ImGui::Begin("Game", &active))
	{
		focused = ImGui::IsWindowFocused();

		ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();

		//Check if window is Resized
		if (current_viewport_size.x != panel_size.x || current_viewport_size.y != panel_size.y)
		{
			panel_size = current_viewport_size;

			App->renderer3D->CanResize(App->renderer3D->GetGameViewport());
			App->renderer3D->OnResize(panel_size.x, panel_size.y, App->camera->GetGameCamera());

		}
		panel_pos = ImGui::GetWindowPos();
	
		// Render inside Window
		if(App->renderer3D->is_zbuffer_active)
			ImGui::Image((ImTextureID)App->renderer3D->game_viewport->z_buffer, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
		else
			ImGui::Image((ImTextureID)App->renderer3D->game_viewport->render_texture, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
		
	}
	ImGui::End();
	ImGui::PopStyleVar();

	App->gui->scene->active_preview = false;
}

const ImVec2 PanelGame::GetPanelSize()
{
	return panel_size;
}

const ImVec2 PanelGame::GetPanelPos()
{
	return panel_pos;
}
