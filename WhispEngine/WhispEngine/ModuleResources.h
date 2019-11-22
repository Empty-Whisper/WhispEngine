#pragma once
#include "Module.h"
#include "Resource.h"
#include "Globals.h"

class ResourceTexture;

class ModuleResources :
	public Module
{
public:
	ModuleResources();
	~ModuleResources();

	bool CleanUp() override;

	uint64 Find(const char* assets_file) const;
	uint64 ImportFile(const char* file_path);
	Resource* CreateResource(Resource::Type type, const uint64 &force_uid = 0);

	bool LoadToMemory(const uint64& uid);
	bool FreeMemory(const uint64& uid);

	const Resource* Get(const uint64& uid) const;
		  Resource* Get(const uint64& uid);

	bool GetTextures(std::vector<ResourceTexture*>& to_fill);

private:
	std::map<uint64_t, Resource*> resources;
};

