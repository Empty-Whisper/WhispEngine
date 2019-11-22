#include "PanelResources.h"
#include "Imgui/imgui.h"
#include <experimental/filesystem>
#include "Application.h"
#include "Globals.h"

#include "ModuleImport.h"

PanelResources::PanelResources()
{
}


PanelResources::~PanelResources()
{
}

void PanelResources::Update()
{
	if (ImGui::Begin("Resources", &active))
	{
		if (ImGui::TreeNodeEx(ASSETS_FOLDER)) {
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
					FileSystem::Format format = App->dummy_file_system->GetFormat(entry.path().extension().u8string().c_str());
					switch (format)
					{
					case FileSystem::Format::JPG:
					case FileSystem::Format::PNG:
					case FileSystem::Format::DDS:
					case FileSystem::Format::FBX:
						if (!App->dummy_file_system->Exists((entry.path().u8string() + ".meta").c_str())) {
							if (App->importer->Import(entry.path().u8string().c_str()) == false) {
								LOG("FAILED to import %s", entry.path().u8string());
							}
						}
						App->importer->Import((entry.path().u8string() + ".meta").c_str());
						break;
					case FileSystem::Format::SCENE:
						App->LoadScene(entry.path().u8string().c_str());
						break;
					default:
						break;
					}
				}
				ImGui::TreePop();
			}
	}
}
