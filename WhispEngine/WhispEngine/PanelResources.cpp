#include "PanelResources.h"
#include "Imgui/imgui.h"
#include <experimental/filesystem>
#include "Application.h"
#include "ModuleResources.h"
#include "Globals.h"

#include "ModuleImport.h"

PanelResources::PanelResources(const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	: Panel("Inspector", start_active, shortcut1, shortcut2, shortcut3)
{
}


PanelResources::~PanelResources()
{
}

void PanelResources::Update()
{
	if (ImGui::Begin("Resources", &active))
	{
		if (ImGui::TreeNodeEx(ASSETS_FOLDER, ImGuiTreeNodeFlags_DefaultOpen)) {
			DrawNode(ASSETS_FOLDER);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void PanelResources::DrawNode(const char * path)
{
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
		if (entry.path().extension().u8string().compare(".meta") != 0)
			if (entry.path().has_extension() ? ImGui::TreeNodeEx(entry.path().filename().u8string().data(), ImGuiTreeNodeFlags_Leaf) : ImGui::TreeNodeEx(entry.path().filename().u8string().data())) {
				if (!entry.path().has_extension())
					DrawNode(entry.path().u8string().data());
				else if (ImGui::IsItemClicked()) {
					FileSystem::Format format = App->file_system->GetFormat(entry.path().extension().u8string().c_str());
					switch (format)
					{
					case FileSystem::Format::FBX:
						if (App->file_system->Exists((entry.path().u8string() + ".meta").c_str())) {
							App->resources->Get(App->file_system->GetUIDFromMeta((entry.path().u8string() + ".meta").c_str()))->LoadToMemory();
						}
						break;
					case FileSystem::Format::SCENE:
						App->LoadScene(entry.path().u8string().c_str());
						break;
					case FileSystem::Format::LUA:

						break;
					default:
						break;
					}
				}
				ImGui::TreePop();
			}
	}
}
