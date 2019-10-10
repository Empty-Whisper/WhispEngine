#pragma once
#include "Module.h"
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

};

