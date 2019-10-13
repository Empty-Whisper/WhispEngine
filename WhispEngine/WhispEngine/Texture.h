#pragma once

#include "Globals.h"
#include <string>

class Texture
{
public:
	Texture(const uint &id, const char* name, const int& width, const int& height);
	~Texture();

	uint id = 0;
	std::string name;
	std::string path;
	
	int width = -1;
	int height = -1;
};

