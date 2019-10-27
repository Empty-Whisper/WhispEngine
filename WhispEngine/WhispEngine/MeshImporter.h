#pragma once
#include "Importer.h"
class MeshImporter :
	public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Import(const char* path);
};

