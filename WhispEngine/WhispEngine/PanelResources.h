#pragma once
#include "Panel.h"
#include <vector>
#include "FileSystem.h"

class PanelResources :
	public Panel
{
private:
	struct File {
		PanelResources* panel = nullptr;
		bool is_folder = false;
		std::string path;
		std::string name;
		std::vector<File*> children;
		File* parent = nullptr;
		FileSystem::Format format = FileSystem::Format::NONE;

		File(bool is_folder, const char* path, File* parent, FileSystem::Format format, PanelResources* panel);

		~File() {
			if (parent != nullptr)
				parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));

			if (!children.empty()) {
				auto cpy_children = children;
				for (auto child = cpy_children.begin(); child != cpy_children.end(); child++) {
					delete *child;
				}
			}
		}

		void Draw();
	};
public:
	PanelResources(const bool & start_active = true, 
		const SDL_Scancode & shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode & shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode & shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelResources();

	void Update();

	void RefreshFiles();

	std::string file_dragdrop;

	bool to_refresh = false;
private:
	void GeneratePanelResources(File* const parent);
	File* files = nullptr;
	File* to_delete = nullptr;
};
