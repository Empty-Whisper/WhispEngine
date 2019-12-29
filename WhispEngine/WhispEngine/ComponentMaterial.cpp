#include "ComponentMaterial.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "MaterialImporter.h"
#include "ResourceTexture.h"
#include "ModuleImport.h"
#include "ModuleInput.h"
#include "ModuleObjectManager.h"
#include "ModuleResources.h"
#include "Resource.h"
#include "mmgr/mmgr.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent, ComponentType::MATERIAL)
{
}


ComponentMaterial::~ComponentMaterial()
{
	App->resources->FreeMemory(uid);
}

const bool ComponentMaterial::HasTexture() const
{
	return uid != 0u;
}

const uint ComponentMaterial::GetIDTexture() const
{
	ResourceTexture *res = (ResourceTexture*)App->resources->Get(uid);
	if (res != nullptr)
		return res->buffer_id;
	return 0u;
}

void ComponentMaterial::OnInspector()
{
	ActiveImGui();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::ColorEdit4("Face Color", face_color);
		ImGui::ColorEdit4("Wireframe Color", wire_color);

		if (uid != 0u) {
			ResourceTexture * texture = (ResourceTexture*)App->resources->Get(uid);
			if (texture != nullptr)
				ImGui::Text("%s", texture->GetFile());
			if (ImGui::Button("Change Texture")) {
				select_tex = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Deselect Texture")) {
				App->resources->FreeMemory(uid);
				uid = 0;
				texture = nullptr;
			}

			if (texture != nullptr) {
				ImGui::Text("(%d, %d)", texture->width, texture->height);
				ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "References: %u", texture->GetReferences());
				ImGui::Image((ImTextureID)texture->buffer_id, ImVec2(128.f, 128.f));
			}
		}
		else {
			if (ImGui::Button("Select Texture")) {
				select_tex = true;
			}
		}
		if (select_tex) {
			float width = 20;
			float height = 20.f;
			if (ImGui::Begin("Select Texture", &select_tex)) {
				std::vector<ResourceTexture*> textures;
				App->resources->GetTextures(textures);
				for (auto i = textures.begin(); i != textures.end(); ++i) {
					if (ImGui::Button(App->file_system->GetFileFromPath((*i)->GetFile()).c_str())) {
						App->resources->LoadToMemory((*i)->uid);
						uid = (*i)->uid;
						select_tex = false;
					}
				}

				ImGui::End();
			}
		}
	}
}

void ComponentMaterial::Save(nlohmann::json & node)
{
	node["Resource"] = uid;
	App->json->AddColor4("face color", face_color, node);
	App->json->AddColor4("wire color", wire_color, node);
}

void ComponentMaterial::Load(const nlohmann::json & node)
{
	uid = (uint64)node.value("Resource", (uint64)0u);
	App->resources->LoadToMemory(uid);

	App->json->GetColor4("face color", node, face_color);

	App->json->GetColor4("wire color", node, wire_color);
}



void ComponentMaterial::SetFaceColor(const float & r, const float & g, const float & b, const float & a)
{
	face_color[0] = r;
	face_color[1] = g;
	face_color[2] = b;
	face_color[3] = a;
}

void ComponentMaterial::SetFaceColor(const float * c)
{
	memcpy(face_color, c, sizeof(float) * 4);
}

void ComponentMaterial::SetWireColor(const float & r, const float & g, const float & b, const float & a)
{
	wire_color[0] = r;
	wire_color[1] = g;
	wire_color[2] = b;
	wire_color[3] = a;
}

void ComponentMaterial::SetWireColor(const float * c)
{
	memcpy(wire_color, c, sizeof(float) * 4);
}

const float * ComponentMaterial::GetFaceColor() const
{
	return &face_color[0];
}

const float * ComponentMaterial::GetWireColor() const
{
	return &wire_color[0];
}
