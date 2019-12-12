#pragma once
#include "Panel.h"
#include "ImGuiColorTextEdit/TextEditor.h"

class PanelScriptEditor :
	public Panel
{
public:
	PanelScriptEditor(const bool &start_active = true, const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelScriptEditor();

	void Update() override;

	void SetFile(const char* new_file);

private:
	TextEditor editor;
	std::string file;
};

