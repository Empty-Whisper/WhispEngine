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
private:

	struct HierarchyInfo {
		uint mesh_id = 0;
		std::vector<HierarchyInfo> children;
	};
public:
	ModelImporter();
	~ModelImporter();

	bool Import(const char* path);

	void FillChildrenInfo(ModelImporter::HierarchyInfo &info, char * &cursor);

	void LoadNode(aiNode * node, GameObject * parent, const aiScene * scene);
	uint CalculateHierarchyInfo(HierarchyInfo* info, const aiNode* node, uint* mesh_id, const std::string & name);
};

