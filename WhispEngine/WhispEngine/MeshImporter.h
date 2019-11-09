#pragma once
#include "Importer.h"
#include <stdint.h>
#include "ComponentMesh.h"

struct aiMesh;

class MeshImporter :
	public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Import(const uint64_t &uid, const aiMesh* mesh);
	bool Load(const uint64_t &uid, Mesh_info * mesh);
};

