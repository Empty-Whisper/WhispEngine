#pragma once
#include "JSON/json.hpp"
#include "MathGeoLib/include/Math/float3.h"

class JsonHelper
{
public:
	JsonHelper();
	~JsonHelper();

public:
	void AddFloat3(const char* name, const float3 &vector, nlohmann::json &node);
	float3 GetFloat3(const char* name, const nlohmann::json &node);

	void AddQuaternion(const char* name, const Quat &vector, nlohmann::json &node);
	Quat GetQuaternion(const char* name, const nlohmann::json &node);

	void AddColor3(const char* name, const float *vector, nlohmann::json &node);
	float* GetColor3(const char* name, const nlohmann::json &node);
	void AddColor4(const char* name, const float *vector, nlohmann::json &node);
	float* GetColor4(const char* name, const nlohmann::json &node);
};

