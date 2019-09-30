#pragma once
#include "JSON/json.hpp"

class FileSystem
{
public:
	enum class Format {
		TO_READ, TO_WRITE, TO_READ_AND_WRITE
	};

public:
	FileSystem();
	~FileSystem();

public:
	nlohmann::json OpenFile(const char* path);
	void SaveFile(const char* path, const nlohmann::json &to_save);
};

