#include "PanelResources.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include <experimental/filesystem>
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleObjectManager.h"
#include "Globals.h"
#include "ComponentScript.h"

#include "ModuleGUI.h"
#include "PanelScriptEditor.h"

#include "ModuleImport.h"
#include "Brofiler/Brofiler.h"

#include <string>

PanelResources::PanelResources(const bool &start_active, const SDL_Scancode &shortcut1, const SDL_Scancode &shortcut2, const SDL_Scancode &shortcut3)
	: Panel("Inspector", start_active, shortcut1, shortcut2, shortcut3)
{
	GeneratePanelResources(files = new File(true, ASSETS_FOLDER, nullptr, FileSystem::Format::NONE, this));
}

void PanelResources::GeneratePanelResources(File* const parent)
{
	for (const auto & entry : std::experimental::filesystem::directory_iterator(parent->path)) {
		if (entry.path().extension().u8string().compare(".meta") != 0 && entry.path().u8string().find("Internal") == std::string::npos) {
			File* file = new File(
				std::experimental::filesystem::is_directory(entry.path()), 
				entry.path().u8string().c_str(), 
				files, 
				App->file_system->GetFormat(entry.path().u8string().c_str()), 
				this
			);
			if (file->is_folder) {
				GeneratePanelResources(file);
			}
			parent->children.push_back(file);
		}
	}
}


PanelResources::~PanelResources()
{
	delete files;
}

void PanelResources::Update()
{
	BROFILER_CATEGORY("Resources", Profiler::Color::Purple);
	if (ImGui::Begin("Resources", &active, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Refresh")) {
				delete files;
				GeneratePanelResources(files = new File(true, ASSETS_FOLDER, nullptr, FileSystem::Format::NONE, this));
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::TreeNodeEx(ASSETS_FOLDER, ImGuiTreeNodeFlags_DefaultOpen)) {
			files->Draw();

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

PanelResources::File::File(bool is_folder, const char * path, const File * parent, FileSystem::Format format, PanelResources * panel)
	: is_folder(is_folder), path(path), parent(parent), format(format), panel(panel)
{
	name.assign(App->file_system->GetFileFromPath(path));
}

void PanelResources::File::Draw()
{
	for (auto file = children.cbegin(); file != children.cend(); file++) {
		if (!(*file)->is_folder ? ImGui::TreeNodeEx((*file)->name.c_str(), ImGuiTreeNodeFlags_Leaf) : ImGui::TreeNodeEx((*file)->name.c_str())) {
			if ((*file)->is_folder)
				(*file)->Draw();
			else if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				switch ((*file)->format)
				{
				case FileSystem::Format::FBX:
					if (App->file_system->Exists(((*file)->path + ".meta").c_str())) {
						App->resources->Get(App->file_system->GetUIDFromMeta(((*file)->path + ".meta").c_str()))->LoadToMemory();
					}
					break;
				case FileSystem::Format::SCENE:
					App->LoadScene((*file)->path.c_str());
					break;
				case FileSystem::Format::LUA:
					App->gui->editor->SetFile((*file)->path.c_str());
					break;
				default:
					break;
				}
			}
			if (!(*file)->is_folder)
				if (ImGui::BeginDragDropSource()) {
					(*file)->panel->file_dragdrop = (*file)->path;
					std::string type;
					switch ((*file)->format) {
					case FileSystem::Format::LUA:
						ImGui::SetDragDropPayload("SCRIPT", &(*file)->panel->file_dragdrop, sizeof(int));
						break;
					}
					ImGui::EndDragDropSource();
				}

			ImGui::TreePop();
		}
	}
}
