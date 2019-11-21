#include "PanelGame.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

PanelGame::PanelGame(const bool & start_active, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3)
	: Panel("Scene", start_active, shortcut1, shortcut2, shortcut3)
{
}
PanelGame::~PanelGame()
{
}

void PanelGame::Update()
{
	if (ImGui::Begin("Game", &active))
	{

	}
	ImGui::End();
}

const ImVec2 PanelGame::GetPanelSize()
{
	return panel_size;

	return ImVec2();
}

const ImVec2 PanelGame::GetPanelPos()
{
	return panel_pos;
}
