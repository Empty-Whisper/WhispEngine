#pragma once
#include "Module.h"

struct aiNode;
struct aiScene;
class GameObject;

class ModuleImport :
	public Module
{
public:
	ModuleImport();
	~ModuleImport();

	bool Start();

	bool CleanUp();

	bool ImportFbx(const char* path);
	bool ImportTexture(const char* path);

private:
	void LoadNode(aiNode * node, GameObject * container, const aiScene * scene);
};

