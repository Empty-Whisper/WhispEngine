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
			ImGui::PushID(*i);
			ImGui::Checkbox("", &(*i)->active); ImGui::SameLine(); 
			if (ImGui::CollapsingHeader("Color")) {
				ImGui::ColorEdit3("Face color", (*i)->color);
				ImGui::ColorEdit3("Wire color", (*i)->wire_color);
			}
			ImGui::PopID();
		}
		ImGui::End();
	}
}
