#pragma once
#include "Importer.h"
#include "Globals.h"
#include <vector>
#include "JSON/json.hpp"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

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
		std::string name;
		float3 position;
		Quat rotation;
		float3 scale;
	};

public:
	ModelImporter();
	~ModelImporter();

	bool Import(const char* path);

	bool Load(const char* path);

	void CreateObjects(GameObject * container, const nlohmann::json & data);

private:

	void FillChildrenInfo(ModelImporter::HierarchyInfo &info, char* &cursor, nlohmann::json & file);
	
	void LoadNode(aiNode * node, GameObject * parent, const aiScene * scene);
	uint CalculateHierarchyInfo(HierarchyInfo* info, const aiNode* node, const aiScene* scene);
};

