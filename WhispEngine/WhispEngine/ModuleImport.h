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

public:
	Texture* logo_txt = nullptr;

private:
	void LoadNode(aiNode * node, GameObject * container, const aiScene * scene);

private:
	ModelImporter*		model = nullptr;
	MaterialImporter*	material = nullptr;
	MeshImporter*		mesh = nullptr;
};

