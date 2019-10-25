#include "ComponentMaterial.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Application.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent, ComponentType::MATERIAL)
{
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::SetTexture(Texture * texture)
{
	this->texture = texture;
}

const bool ComponentMaterial::HasTexture() const
{
	return texture != nullptr;
}

const uint ComponentMaterial::GetIDTexture() const
{
	return texture->id;
}

void ComponentMaterial::OnInspector()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::SameLine();
		ActiveImGui();

		ImGui::ColorEdit4("Face Color", face_color);
		ImGui::ColorEdit4("Wireframe Color", wire_color);

		if (texture != nullptr) {
			ImGui::Text("%s", texture->path.data());
			if (ImGui::Button("Change Texture")) {
				select_tex = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Deselect Texture")) {
				texture = nullptr;
			}

			if (texture != nullptr) {
				ImGui::Text("(%d, %d)", texture->width, texture->height);
				ImGui::Image((ImTextureID)texture->id, ImVec2(128.f, 128.f));
			}
		}
		else {
			if (ImGui::Button("Select Texture")) {
				select_tex = true;
			}
		}
		if (select_tex) {
			float width = 128.f;
			float height = 128.f;
			if (ImGui::Begin("Select Texture", &select_tex)) {
				std::vector<Texture*>* tex = App->object_manager->GetTextures();
				int warp = 1;
				for (auto i = tex->begin(); i != tex->end(); i++) {
					if (ImGui::ImageButton((ImTextureID)(*i)->id, ImVec2(width, height))) {
						texture = *i;
					}
					if (warp % 3 != 0)
						ImGui::SameLine();
					warp++;
				}

				ImGui::End();
			}
		}
	}
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

Texture::Texture(const uint &id, const char* path, const int& width, const int& height)
	: id(id), path(path), name(App->file_system->GetFileNameFromPath(path)), width(width), height(height)
{
}
