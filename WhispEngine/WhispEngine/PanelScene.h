#ifndef _PANEL_SCENE__
#define _PANEL_SCENE__


#include "Panel.h"
#include "Globals.h"
#include "Imgui/imgui.h"

class PanelScene: public Panel
{
public:
	PanelScene(const bool &start_active = true, const SDL_Scancode &shortcut1 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut2 = SDL_SCANCODE_UNKNOWN, const SDL_Scancode &shortcut3 = SDL_SCANCODE_UNKNOWN);
	~PanelScene();

	void Update() override;

	const ImVec2 GetPanelSize();
	
private:
	ImVec2 panel_size = { 0.f, 0.f};

};

#endif // !_PANEL_SCENE__