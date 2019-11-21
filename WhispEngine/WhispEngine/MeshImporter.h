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

	uint64 Import(const char* path, const aiMesh* mesh, const uint64 &parent_uid);
	bool Load(const uint64_t &uid, Mesh_info * mesh);
};

