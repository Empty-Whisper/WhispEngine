#pragma once
#include "Importer.h"
#include "Globals.h"
#include <vector>

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

	void FillChildrenInfo(ModelImporter::HierarchyInfo &info, char * &cursor);

	void LoadNode(aiNode * node, GameObject * parent, const aiScene * scene);
	uint CalculateHierarchyInfo(HierarchyInfo* info, const aiNode* node, const aiScene* scene);
};

