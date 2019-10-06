#pragma once
#include "Panel.h"
class PanelRender :
	public Panel
{
public:
	PanelRender();
	~PanelRender();

	void Update() override;
};

