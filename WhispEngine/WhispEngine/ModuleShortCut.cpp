#include "ModuleShortCut.h"
#include "Application.h"
#include "ModuleInput.h"


ModuleShortCut::ModuleShortCut()
{
	name.assign("ShortCut");
}


ModuleShortCut::~ModuleShortCut()
{
}

update_status ModuleShortCut::PreUpdate()
{
	if (!App->input->block_keyboard) {
		for (auto i = shortcuts.begin(); i != shortcuts.end(); ++i) {
			if ((*i)->IsShortCutPressed()) {
				(*i)->DoFunc();
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleShortCut::AddShortCut(ShortCut * shortcut)
{
	shortcuts.push_back(shortcut);
}
