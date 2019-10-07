#pragma once
#include "Panel.h"
#include <vector>
#include "ModuleObjectManager.h"

class PanelCreate :
	public Panel
{
public:
	PanelCreate();
	~PanelCreate();

	void Update() override;

private:
	std::vector<std::string> items;
	std::vector<std::string>::iterator iterator;

	Object_data data;
};

