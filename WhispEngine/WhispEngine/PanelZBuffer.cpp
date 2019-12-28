#include "PanelZBuffer.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "PanelScene.h"
#include "ModuleGUI.h"
#include "ModuleRenderer3D.h"
#include "Brofiler/Brofiler.h"

PanelZBuffer::PanelZBuffer(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3)
	: Panel("zBuffer", start_active, shortcut1, shortcut2, shortcut3)
{
}
PanelZBuffer::~PanelZBuffer()
{
}

void PanelZBuffer::Update()
{
	BROFILER_CATEGORY("zBuffer", Profiler::Color::Moccasin);

	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (ImGui::Begin("zBuffer", &active))
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
			ImGui::Image((ImTextureID)App->renderer3D->game_viewport->z_buffer, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
		
	}
	ImGui::End();
	ImGui::PopStyleVar();

	App->gui->scene->active_preview = false;
}

const ImVec2 PanelZBuffer::GetPanelSize()
{
	return panel_size;
}

const ImVec2 PanelZBuffer::GetPanelPos()
{
	return panel_pos;
}
