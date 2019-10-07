#include "PanelRender.h"
#include "Application.h"


PanelRender::PanelRender()
{
}


PanelRender::~PanelRender()
{
}

void PanelRender::Update()
{
	if (ImGui::Begin("Render", &active)) {
		ImGui::Checkbox("Wireframe", &App->renderer3D->wireframe);
		ImGui::Checkbox("Fill faces", &App->renderer3D->fill);
		ImGui::Checkbox("Normals", &App->renderer3D->see_normals); ImGui::Checkbox("My Normals", &App->renderer3D->my_normals);
		ImGui::Separator();
		ImGui::Checkbox("Show grid", &App->scene_intro->show_grid);
		ImGui::ColorPicker3("Background Color", App->renderer3D->background_color);
		ImGui::Separator();

		ImGui::End();
	}
}
