#include "PanelResources.h"
#include "Imgui/imgui.h"
#include <experimental/filesystem>
#include "Application.h"
#include "Globals.h"

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
		if (ImGui::TreeNodeEx(LIBRARY_FOLDER)) {
			DrawNode(LIBRARY_FOLDER);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void PanelResources::DrawNode(const char * path)
{
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
		if (entry.path().has_extension() ? ImGui::TreeNodeEx(entry.path().filename().u8string().data(), ImGuiTreeNodeFlags_Leaf) : ImGui::TreeNodeEx(entry.path().filename().u8string().data())) {
			if (!entry.path().has_extension())
				DrawNode(entry.path().u8string().data());
			else if (ImGui::IsItemClicked()) {
				if (App->dummy_file_system->GetFormat(entry.path().extension().u8string().c_str()) == FileSystem::Format::MODEL) {
					App->importer->Import(entry.path().u8string().c_str());
				}
			}
			ImGui::TreePop();
		}
	}
}
