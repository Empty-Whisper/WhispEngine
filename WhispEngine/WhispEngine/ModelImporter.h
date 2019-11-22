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
	

public:
	ModelImporter();
	~ModelImporter();

	uint64 Import(const char* path);

	//bool Load(const char* path);

	//void CreateObjects(GameObject * container, const nlohmann::json & data);
};

