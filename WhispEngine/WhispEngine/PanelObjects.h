#pragma once
#include "Panel.h"
#include "GameObject.h"
class PanelObjects :
	public Panel
{
public:
	PanelObjects();
	~PanelObjects();

	void Update() override;

	const char* normal_name[(int)Normals::MAX] = { "No view","Faces","Vertex" };
};

