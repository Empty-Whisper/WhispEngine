#pragma once
#include "Module.h"

struct aiNode;
struct aiScene;
class GameObject;
class Texture;
class ModelImporter;
class MaterialImporter;
class MeshImporter;

class ModuleImport :
	public Module
{
	friend class ModelImporter;
	friend class MeshImporter;

public:
	ModuleImport();
	~ModuleImport();

	bool Start();

	bool CleanUp();

	bool Import(const char* path);

private:
	void CreateLibrary();
	void CreateFiles(const char* directory);


public:
	Texture* logo_txt = nullptr;

public:
	ModelImporter*		model = nullptr;
	MaterialImporter*	material = nullptr;
	MeshImporter*		mesh = nullptr;
};

