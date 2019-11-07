#pragma once
#include "Importer.h"
#include <stdint.h>

struct aiMesh;

class MeshImporter :
	public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Import(const uint64_t &uid, const aiMesh* mesh);
};

