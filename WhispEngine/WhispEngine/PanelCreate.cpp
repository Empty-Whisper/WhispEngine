#include "PanelCreate.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h" // for ImGui::PushFlag
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

		ImGui::PushItemWidth(100);
		ImGui::PushID("pos");
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &data.pos.x, 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &data.pos.y, 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &data.pos.z, 0.f, 5.f);
		ImGui::PopID();

		ImGui::Separator();

		ImGui::PushID("rot");
		ImGui::Text("Rotation");
		ImGui::SliderFloat("X", &data.rotate.axis[0], 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &data.rotate.axis[1], 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &data.rotate.axis[2], 0.f, 5.f);
		ImGui::SliderFloat("Angle (rad)", &data.rotate.angle, 0.f, 6.28);
		ImGui::PopID();

		ImGui::Separator();

		ImGui::PushID("scale");
		ImGui::Text("Scale");
		ImGui::SliderFloat("X", &data.scale.x, 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &data.scale.y, 0.f, 5.f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &data.scale.z, 0.f, 5.f);
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::NewLine();

		switch (Primitives(iterator-items.begin()))
		{
		case Primitives::SPHERE:
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::SliderFloat("Radius", &data.radius, 1.f, 5.f);
			ImGui::SliderInt("Rings", &data.rings, 1, 50);
			ImGui::SliderInt("Sectors", &data.slices, 1, 50);
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();

			ImGui::SameLine(); ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("That items will replace items below");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}



			break;
		case Primitives::HEMISPHERE:
			break;
		case Primitives::TORUS:
			break;
		case Primitives::CONE:
			break;
		case Primitives::CYLINDER:
			break;
		case Primitives::MAX:
			break;
		default:
			break;
		}

		/*if (ImGui::Button("Create")) {
			App->object_manager->CreatePrimitive((Primitives)(iterator - items.begin()));
		}*/


		ImGui::End();
	}
}
