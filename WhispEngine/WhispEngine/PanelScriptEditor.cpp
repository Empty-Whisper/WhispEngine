#include "PanelScriptEditor.h"
#include <fstream>

PanelScriptEditor::PanelScriptEditor(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3)
{
	editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

	std::ifstream t("Assets/Scripts/test.lua");
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}
}

PanelScriptEditor::~PanelScriptEditor()
{
}

void PanelScriptEditor::Update()
{
	if (ImGui::Begin("Script Editor", &active)) {
		auto cpos = editor.GetCursorPosition();
		
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), "Assets/Scripts/test.lua");

		editor.Render("TextEditor");
		
		ImGui::End();
	}
}


