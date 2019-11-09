#pragma once
#include "Importer.h"
#include "Globals.h"
#include <vector>

#define SIZE_OF_NODE sizeof(uint) + sizeof(char) + sizeof(uint64_t)

class GameObject;
struct aiNode;
struct aiScene;

class ModelImporter :
	public Importer
{
public:
	struct HierarchyInfo {
		uint64_t mesh_id = 0;
		HierarchyInfo* parent = nullptr;
		std::vector<HierarchyInfo> children;
	};

public:
	ModelImporter();
	~ModelImporter();

	bool Import(const char* path);

	bool Load(const char* path);

	void CreateObjects(GameObject * container, char * &cursor);

private:

	void FillChildrenInfo(ModelImporter::HierarchyInfo &info, char* &cursor);

	void LoadNode(aiNode * node, GameObject * parent, const aiScene * scene);
	uint CalculateHierarchyInfo(HierarchyInfo* info, const aiNode* node, const aiScene* scene);
};

