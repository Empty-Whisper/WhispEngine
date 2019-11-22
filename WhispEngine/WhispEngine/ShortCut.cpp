#include "ShortCut.h"
#include "Application.h"
#include "ModuleShortCut.h"
#include "ModuleInput.h"

ShortCut::ShortCut()
{
	App->shortcut->AddShortCut(this);
}

ShortCut::ShortCut(const std::function<void()> &func, const SDL_Scancode & shortcut1, const SDL_Scancode & shortcut2, const SDL_Scancode & shortcut3)
	: shortcut{ shortcut1, shortcut2, shortcut3 }, function(func)
{
	App->shortcut->AddShortCut(this);
}

ShortCut::~ShortCut()
{
}

void ShortCut::SetShortCut(const SDL_Scancode & key1, const SDL_Scancode & key2, const SDL_Scancode & key3)
{
	shortcut[0] = key1;
	shortcut[1] = key2;
	shortcut[2] = key3;
}

bool ShortCut::IsShortCutPressed() const
{
	bool ret = false;

	if (shortcut[2] == SDL_SCANCODE_UNKNOWN) {
		if (shortcut[1] == SDL_SCANCODE_UNKNOWN) {
			ret = App->input->GetKeyDown(shortcut[0]);
		}
		else {
			ret = (App->input->GetKey(shortcut[0]) && App->input->GetKeyDown(shortcut[1]));
		}
	}
	else {
		ret = (App->input->GetKey(shortcut[0]) && App->input->GetKey(shortcut[1]) && App->input->GetKeyDown(shortcut[2]));
	}

	return ret;
}

void ShortCut::DoFunc()
{
	function();
}
