#pragma once
#include "Resource.h"
class ResourceTexture :
	public Resource
{
public:
	ResourceTexture(const uint64& uid);
	~ResourceTexture();

	bool Set(const uint &width, const uint &height, const uint &id);

	bool LoadInMemory() override;
	bool FreeMemory() override;

protected:
	uint buffer_id = 0;
private:
	uint width = 0;
	uint height = 0;
	uint depth = 0;
	uint mips = 0;
	uint bytes = 0;
};

