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

				enum SliderElement { NONE, SHOW_NORMALS, SHOW_VERTEX_NORMALS, Element_COUNT };
				const char* element_names[Element_COUNT] = { "None", "Show Normals", "Show Vertex Normals"};
				static int current_element = NONE;
				const char* current_element_name = (current_element >= 0 && current_element < Element_COUNT) ? element_names[current_element] : "Unknown";

				ImGui::SliderInt("Set Normals", &current_element, 0, Element_COUNT - 1, current_element_name);
				//(*i)->normals = &current_element;
				if (current_element == 0) App->renderer3D->see_normals = false;
				if (current_element == 1) App->renderer3D->see_normals = true;
				if (current_element == 2) App->renderer3D->see_normals = false;
			}
			ImGui::PopID();
		}
		ImGui::End();
	}
}
