#include "Resource.h"

Resource::Resource(const uint64 & uid, Type type) : uid(uid), type(type)
{
}

Resource::~Resource()
{
}

Resource::Type Resource::GetType() const
{
	return type;
}

uint64 Resource::GetUID() const
{
	return uid;
}

const char * Resource::GetFile() const
{
	return file.c_str();
}

const char * Resource::GetLibraryPath() const
{
	return resource_path.c_str();
}

bool Resource::IsLoadedInMemory() const
{
	return references > 0u;
}

bool Resource::LoadToMemory()
{
	bool success = true;

	if (references != 0u)
		references++;
	else {
		 success = LoadInMemory();
		if(success)
			references++;
	}

	return success;
}

bool Resource::FreeFromMemory()
{
	if (references > 0)
		return FreeMemory();
	return true;
}

uint Resource::GetReferences() const
{
	return references;
}

void Resource::SetFile(const char * path)
{
	file.assign(path);
}

void Resource::SetResourcePath(const char * path)
{
	resource_path.assign(path);
}
