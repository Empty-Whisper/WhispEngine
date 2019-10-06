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
	if (ImGui::Begin("About Whisp Engine", &active)) {
		ImGui::Checkbox("Wireframe", &App->renderer3D->wireframe);
		ImGui::Checkbox("Fill faces", &App->renderer3D->fill);
		ImGui::Separator();
		ImGui::Checkbox("Show grid", &App->scene_intro->show_grid);
		ImGui::Separator();

		ImGui::End();
	}
}
