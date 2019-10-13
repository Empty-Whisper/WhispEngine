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

	math::float4x4 GetLocalMatrix() const;
	math::float4x4 GetGlobalMatrix() const; //parent’s global matrix * your local Matrix

public:

	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::zero;


};
#endif
