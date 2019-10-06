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

	bool ImportFile(const char* path);

};

