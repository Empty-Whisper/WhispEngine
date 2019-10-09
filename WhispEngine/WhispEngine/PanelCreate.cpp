#include "PanelCreate.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h" // for ImGui::PushFlag
#include "Application.h"
#include "Globals.h"

PanelCreate::PanelCreate(/*const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	:Panel("Create", start_active, shortcut1, shortcut2, shortcut3*/)
{
	items.resize((int)Primitives::MAX);
	for (int i = 0; i < (int)Primitives::MAX; ++i) {
		switch ((Primitives)i)
		{
		case Primitives::CUBE:
			items[i] = "Cube";
			break;
		case Primitives::TETRAHEDRON:
			items[i] = "Tetrahedron";
			break;
		case Primitives::OCTAHEDRON:
			items[i] = "Octahedron";
			break;
		case Primitives::DODECAHEDRON:
			items[i] = "Dodecahedron";
			break;
		case Primitives::ICOSAHEDRON:
			items[i] = "Icosahedron";
			break;
		case Primitives::SPHERE:
			items[i] = "Sphere";
			break;
		case Primitives::HEMISPHERE:
			items[i] = "Hemisphere";
			break;
		case Primitives::TORUS:
			items[i] = "Torus";
			break;
		case Primitives::CONE:
			items[i] = "Cone";
			break;
		case Primitives::CYLINDER:
			items[i] = "Cylinder";
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

	if (ImGui::Begin("Create", &active)) {
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
		ImGui::SliderFloat("X", &data.pos.x, -15.f, 15.f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &data.pos.y, -15.f, 15.f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &data.pos.z, -15.f, 15.f);
		ImGui::PopID();

		ImGui::Separator();

		ImGui::PushID("rot");
		ImGui::Text("Rotation");
		ImGui::SliderFloat("X", &data.rotate.axis[0], 0.f, 1.f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &data.rotate.axis[1], 0.f, 1.f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &data.rotate.axis[2], 0.f, 1.f);
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
		ImGui::Separator();

		ImGui::NewLine();

		switch (Primitives(iterator-items.begin()))
		{
		case Primitives::SPHERE:
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::SliderFloat("Radius", &data.radius, 0.1f, 5.f);
			ImGui::SliderInt("Rings", &data.rings, 3, 50);
			ImGui::SliderInt("Sectors", &data.slices, 3, 50);
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
			ImGui::SliderInt("Slices", &data.slices, 3, 20);
			ImGui::SliderInt("Rings", &data.rings, 3, 20);
			break;
		case Primitives::TORUS:
			ImGui::SliderInt("Slices", &data.slices, 3, 20);
			ImGui::SliderInt("Rings", &data.rings, 3, 20);
			ImGui::SliderFloat("Radius", &data.radius, 0.1f, 0.9999f);
			break;
		case Primitives::CONE:
			ImGui::SliderInt("Slices", &data.slices, 3, 20);
			ImGui::SliderInt("Rings", &data.rings, 3, 20);
			break;
		case Primitives::CYLINDER:
			ImGui::SliderInt("Slices", &data.slices, 3, 20);
			ImGui::SliderInt("Rings", &data.rings, 3, 20);
			break;
		default:
			break;
		}

		ImGui::ColorEdit3("Face color", data.face_color);
		ImGui::ColorEdit3("Wire color", data.wire_color);

		if (ImGui::Button("Create")) {
			App->object_manager->CreatePrimitive((Primitives)(iterator - items.begin()), data);
		}


		ImGui::End();
	}
}
