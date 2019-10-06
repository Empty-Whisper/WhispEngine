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
		for (auto i = vec->begin(); i != vec->end(); ++i) {
			ImGui::Checkbox("", &(*i)->active); ImGui::SameLine(); ImGui::ColorEdit3("", (*i)->color);
		}


		ImGui::End();
	}
}
