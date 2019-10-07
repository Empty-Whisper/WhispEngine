#include "PanelObjects.h"
#include "Application.h"
#include "Imgui/imgui.h"


PanelObjects::PanelObjects()
{
}


PanelObjects::~PanelObjects()
{
}

void PanelObjects::Update()
{
	if (ImGui::Begin("Inspector", &active)) {
		auto vec = App->object_manager->GetObjects();
		int cont = 0;
		for (auto i = vec->begin(); i != vec->end(); ++i) {
			ImGui::Checkbox(std::to_string(cont).data(), &(*i)->active); ImGui::SameLine(); ImGui::ColorEdit3(std::to_string(cont).data(), (*i)->color);
			cont++;
		}


		ImGui::End();
	}
}
