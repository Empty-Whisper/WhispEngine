#pragma once
#include "Resource.h"

template <typename T>
struct Buffer {
	uint id = 0;
	uint size = 0;
	T* data = nullptr;
};

class ResourceMesh :
	public Resource
{
	friend class MeshImporter;
public:
	ResourceMesh(const uint64& uid);
	~ResourceMesh();

	bool LoadInMemory() override;
	bool FreeMemory() override;

	void SetGLBuffers();

	std::string name;

	Buffer<float> vertex;
	Buffer<uint> index;
	Buffer<float> face_normals;
	Buffer<float> vertex_normals;
	Buffer<float> tex_coords;

protected:
	uint64 parent = 0u;
};

