#pragma once
#include "Resource.h"
#include <vector>
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "JSON/json.hpp"

class GameObject;
struct aiNode;
struct aiScene;

class ResourceModel :
	public Resource
{
public:
	struct HierarchyInfo {
		std::string name;
		uint64 mesh = 0u;
		uint64 material = 0u;
		HierarchyInfo* parent = nullptr;
		std::vector<HierarchyInfo> children;
		float3 position = float3::zero;
		Quat rotation = Quat::identity;
		float3 scale = float3::one;
	};

public:
	ResourceModel(const uint64& uid);
	~ResourceModel();

	HierarchyInfo CalculateHierarchy(const aiNode* node, const aiScene* scene, const std::vector<uint64>& meshes, const std::vector<uint64>& materials, HierarchyInfo * node_from);

	bool LoadInMemory() override;
	bool FreeMemory() override;

	bool Save();

private:
	void SaveNode(const HierarchyInfo& parent, nlohmann::json & node);
	void CreateObjects(GameObject * container, const nlohmann::json & data);

public:
	HierarchyInfo model;
};

