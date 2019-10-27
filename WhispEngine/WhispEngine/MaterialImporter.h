#pragma once
#include "Importer.h"
class MaterialImporter :
	public Importer
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* path);
};

