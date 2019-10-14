#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "Globals.h"

#include "MathGeoLib\include\Math\Quat.h"
#include "MathGeoLib\include\Math\float3.h"
#include "MathGeoLib\include\Math\float4x4.h"

#pragma once
class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	void PreUpdate(); //TODO: 1. Recalculate all the global matrices at the beginning of the frame

	void OnInspector();

	void SetPosition(const float& x, const float& y, const float& z);
	void SetRotation(const float& w, const float& x, const float& y, const float& z);
	void SetScale(const float& x, const float& y, const float& z);

	void SetLocalMatrix(const math::float4x4 &matrix);
	
	void CalculeLocalMatrix();
	void CalculateGlobalMatrix();

	math::float4x4 GetLocalMatrix() const;
	math::float4x4 GetGlobalMatrix() const;
	const float* GetPtrGlobalMatrix() const;

private:
	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::one;

	math::float4x4 local_matrix = math::float4x4::identity;
	math::float4x4 global_matrix = math::float4x4::identity;

};
#endif
