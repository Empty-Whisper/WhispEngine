#pragma once
#include "Panel.h"
class PanelResources :
	public Panel
{
public:
	PanelResources();
	~PanelResources();

	void Update();

private:
	void DrawNode(const char* path);
};

