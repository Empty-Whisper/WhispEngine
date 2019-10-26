#pragma once
#include "JSON/json.hpp"

class FileSystem
{
public:
	enum class Format {
		NONE = -1,
		JSON, FBX, DDS, PNG, JPG,
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
};
