#pragma once
#include "Importer.h"
#include "SDL/include/SDL_config.h"

struct Texture;

class MaterialImporter :
	public Importer
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* path, uint64_t * const uid = nullptr);
	Texture* Load(const char* path);
	Texture* Load(const uint64_t &uid);
};

