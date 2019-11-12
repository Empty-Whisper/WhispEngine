#pragma once
#include "JSON/json.hpp"
#include "Globals.h"

class FileSystem
{
public:
	enum class Format {
		NONE = -1,
		JSON, FBX, DDS, PNG, JPG,
		MODEL, MESH, 
		MAX
	};

public:
	FileSystem();
	~FileSystem();

public:
	nlohmann::json	OpenFile(const char* path);
	void			SaveFile(const char* path, const nlohmann::json &to_save);

	FileSystem::Format GetFormat(const char* file);
	std::string GetFileNameFromPath(const char* file);
	std::string GetFileFromPath(const char* file);

	bool IsInDirectory(const char* directory, const char* file);

	bool SaveData(const char* data, const uint &size, const char* path);

	/*
	Give Data from a file. The result is a char array. WARNING: REMEMBER TO DELETE ARRAY AFTER USE IT
	*/
	char* GetData(const char* path);
};
