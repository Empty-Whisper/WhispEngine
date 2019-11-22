#pragma once
#include "JSON/json.hpp"
#include "Globals.h"
#include "Resource.h"

class FileSystem
{
public:
	enum class Format {
		NONE = -1,
		JSON, FBX, 
		DDS, PNG, JPG, TGA,
		MODEL, MESH, META, SCENE,
		MAX
	};

public:
	FileSystem();
	~FileSystem();

public:
	nlohmann::json	OpenFile(const char* path);
	void			SaveFile(const char* path, const nlohmann::json &to_save);

	FileSystem::Format GetFormat(const char* file) const;
	Resource::Type	   GetResourceType(const char* file) const;
	std::string GetPathFormat(const char* path) const;
	std::string GetFileNameFromPath(const char* file) const;
	std::string GetFileFromPath(const char* file) const;
	std::string GetFileDirectory(const char* file_path) const;

	bool IsFileSupported(const char* path) const;

	bool CreateDir(const char* path);
	bool RemoveFile(const char* path);
	bool RemoveDirAndContent(const char* path);

	bool IsInDirectory(const char* directory, const char* file) const;
	bool IsInSubDirectory(const char* directory, const char* file, std::string * const real_path = nullptr) const;
	
	bool Exists(const char* path) const;

	bool IsMetaVaild(const char* path);
	bool HasMeta(const char* file);
	uint64_t GenerateMetaFile(const char* meta_path);
	uint64_t GenerateMetaFile(const char* meta_path, const uint64& force_uid, const void * addition_data = nullptr, const uint& size_addition_data = 0u);
	uint64_t GetUIDFromMeta(const char* mata_path) const;

	bool SaveData(const void* data, const uint &size, const char* path);

	/*
	Give Data from a file. The result is a char array. WARNING: REMEMBER TO DELETE ARRAY AFTER USE IT
	*/
	char* GetData(const char* path);

private:
	bool RecursiveDirectory(const char* path, const char * file, std::string * const real_path) const;
};
