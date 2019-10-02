#pragma once
#include "Module.h"
#include "ShortCut.h"
#include <vector>

class ModuleShortCut :
	public Module
{
public:
	ModuleShortCut();
	~ModuleShortCut();

	update_status PreUpdate() override;

	void AddShortCut(ShortCut* shortcut);

private:
	std::vector<ShortCut*> shortcuts;
};

