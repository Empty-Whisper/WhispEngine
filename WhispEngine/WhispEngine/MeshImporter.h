#pragma once
#include "Importer.h"
#include <stdint.h>
#include "ComponentMesh.h"

struct aiMesh;
struct aiScene;

class MeshImporter :
	public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	uint64 Import(const char* path, const aiMesh* mesh, const uint64 &parent_uid, const uint64 &force_uid = 0u);
};

