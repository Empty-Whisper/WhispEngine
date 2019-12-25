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
		const File* parent = nullptr;
		FileSystem::Format format = FileSystem::Format::NONE;

		File(bool is_folder, const char* path, const File* parent, FileSystem::Format format, PanelResources* panel);

		~File() {
			for (auto child = children.begin(); child != children.end(); child++) {
				delete *child;
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
private:
	void GeneratePanelResources(File* const parent);
	File* files = nullptr;
};
