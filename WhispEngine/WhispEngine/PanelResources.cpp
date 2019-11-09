#include "PanelResources.h"
#include "Imgui/imgui.h"
#include <experimental/filesystem>
#include "ModuleImport.h"
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
		if (ImGui::TreeNodeEx(LIBRARY_LFOLDER)) {
			DrawNode(LIBRARY_LFOLDER);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void PanelResources::DrawNode(const char * path)
{
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
		if (ImGui::TreeNodeEx(entry.path().filename().u8string().data())) {
			if (!entry.path().has_extension())
				DrawNode(entry.path().u8string().data());
			ImGui::TreePop();
		}
}
