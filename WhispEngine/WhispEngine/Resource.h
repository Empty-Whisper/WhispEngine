#pragma once
#include "Globals.h"
#include <string>

class Resource
{
	friend class ModuleResources;
public:
	enum Type {
		NONE = -1,
		MODEL,//?
		MESH,
		TEXTURE,
		SCENE, // ?????????

		MAX
	};

public:
	Resource(const uint64& uid, Type type);
	virtual ~Resource();

	Type GetType() const;
	uint64 GetUID() const;
	const char* GetFile() const;
	const char* GetLibraryPath() const;
	bool IsLoadedInMemory() const;
	bool LoadToMemory();
	bool FreeFromMemory();
	uint GetReferences() const;
	void SetFile(const char* path);
	void SetResourcePath(const char* path);

	virtual bool LoadInMemory() = 0;
	virtual bool FreeMemory() = 0;

protected:
	uint64 uid = 0U;

	std::string file;			// In Assets/...
	std::string resource_path;  // In Library/...

	uint references = 0U;
	Type type = Type::NONE;
};

