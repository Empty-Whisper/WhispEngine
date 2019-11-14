#pragma once
#include "Importer.h"
#include "SDL/include/SDL_config.h"

class MaterialImporter :
	public Importer
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* path, uint64_t * const uid = nullptr);
};

