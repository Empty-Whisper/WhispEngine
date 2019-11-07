#pragma once
#include "Module.h"

#define ASSETS_AFOLDER "Assets/"
#define MODEL_AFOLDER "Assets/Models/"
#define TEXTURE_AFOLDER "Assets/Textures/"

#define LIBRARY_LFOLDER "Library/"
#define MODEL_LFOLDER "Library/Models/"
#define MESH_LFOLDER "Library/Meshes/"
#define MATERIAL_LFOLDER "Library/Material/"

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

public:
	ModuleImport();
	~ModuleImport();

	bool Start();

	bool CleanUp();

	bool Import(const char* path);
	bool ImportFbx(const char* path);
	Texture* ImportTexture(const char* path);

public:
	Texture* logo_txt = nullptr;

private:
	void LoadNode(aiNode * node, GameObject * container, const aiScene * scene);

private:
	ModelImporter*		model = nullptr;
	MaterialImporter*	material = nullptr;
	MeshImporter*		mesh = nullptr;
};

