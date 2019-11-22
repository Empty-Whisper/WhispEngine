#include "JsonHelper.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float4.h"

JsonHelper::JsonHelper()
{
}


JsonHelper::~JsonHelper()
{
}

void JsonHelper::AddFloat3(const char * name, const float3 & vector, nlohmann::json & node)
{
	node[name] = {
		vector.x, vector.y, vector.z
	};
}

float3 JsonHelper::GetFloat3(const char * name, const nlohmann::json & node)
{
	return float3(node[name][0], node[name][1], node[name][2]);
}

void JsonHelper::AddQuaternion(const char * name, const Quat & quat, nlohmann::json & node)
{
	node[name] = {
		quat.w, quat.x, quat.y, quat.z
	};
}

Quat JsonHelper::GetQuaternion(const char * name, const nlohmann::json & node)
{
	return Quat(node[name][1], node[name][2], node[name][3], node[name][0]);
}

void JsonHelper::AddColor4(const char * name, const float * vector, nlohmann::json & node)
{
	node[name] = {
		vector[0], vector[1], vector[2], vector[3]
	};
}

float * JsonHelper::GetColor4(const char * name, const nlohmann::json & node)
{
	float ret[] = { node[name][0], node[name][1], node[name][2], node[name][3] };

	return ret;
}

void JsonHelper::AddColor3(const char * name, const float * vector, nlohmann::json & node)
{
	node[name] = {
		vector[0], vector[1], vector[2]
	};
}

float * JsonHelper::GetColor3(const char * name, const nlohmann::json & node)
{
	float ret[] = { node[name][0], node[name][1] ,node[name][2] };

	return ret;
}
