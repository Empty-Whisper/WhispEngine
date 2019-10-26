#pragma once
#include "Module.h"

struct aiNode;
struct aiScene;
class GameObject;
class Texture;

class ModuleImport :
	public Module
{
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
};

