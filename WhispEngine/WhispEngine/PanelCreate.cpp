#include "PanelCreate.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "Globals.h"

PanelCreate::PanelCreate()
{
	items.resize((int)Primitives::MAX);
	for (int i = 0; i < (int)Primitives::MAX; ++i) {
		switch ((Primitives)i)
		{
		case Primitives::NONE:
			break;
		case Primitives::CUBE:
			items[i] = "Cube";
			break;
		case Primitives::SPHERE:
			items[i] = "Sphere";
			break;
		case Primitives::MAX:
			break;
		default:
			LOG("Added more primitives than expected, add the missing primitives to the for");
			break;
		}
	}
	iterator = items.begin() + (int)Primitives::CUBE;
}


PanelCreate::~PanelCreate()
{
}

void PanelCreate::Update()
{

	if (ImGui::Begin("Create")) {
		if (ImGui::BeginCombo("Primitive", (*iterator).data())) {
			for (int n = 0; n < items.size(); n++)
			{
				if (ImGui::Selectable(items[n].data(), iterator - items.begin() == n))
					iterator = items.begin() + n;
			}
			ImGui::EndCombo();
		}

		ImGui::NewLine();
		ImGui::Text("Position");
		static float a[3];
		ImGui::PushItemWidth(100);
		ImGui::SliderFloat("X", &a[0], 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &a[1], 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &a[2], 0.f, 5.f); ImGui::SameLine();
		ImGui::PopItemWidth();

		ImGui::NewLine();
		static float width;
		switch ((Primitives)(iterator - items.begin()))
		{
		case Primitives::CUBE:
			ImGui::SliderFloat("Width", &width, 0.1f, 50.f);
			break;
		case Primitives::SPHERE:
			ImGui::SliderFloat("Radious", &width, 0.1f, 50.f);
			break;
		}

		if (ImGui::Button("Create")) {
			App->object_manager->CreatePrimitive((Primitives)(iterator - items.begin()));
		}


		ImGui::End();
	}
}
