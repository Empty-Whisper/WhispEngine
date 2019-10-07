#pragma once
#include "Panel.h"
#include <vector>
class PanelCreate :
	public Panel
{
public:
	PanelCreate();
	~PanelCreate();

	void Update() override;

	std::vector<std::string> items;
	std::vector<std::string>::iterator iterator;
};

