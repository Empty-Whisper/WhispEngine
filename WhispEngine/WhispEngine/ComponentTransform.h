#pragma once

#include "Component.h"
#include "Globals.h"

#include "MathGeoLib\include\Math\Quat.h"
#include "MathGeoLib\include\Math\float3.h"
#include "MathGeoLib\include\Math\float4x4.h"

class GameObject;

class ComponentTransform : public Component
{
	friend class ModuleObjectManager;
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	void PreUpdate();

	void OnInspector();

	void SetPosition(const float& x, const float& y, const float& z);
	void SetPosition(const float3 & pos);
	void SetRotation(const float& w, const float& x, const float& y, const float& z);
	void SetRotation(const Quat & rot);
	void SetScale(const float& x, const float& y, const float& z);
	void SetScale(const float3 & _scale);

	void SetLocalMatrix(const math::float4x4 &matrix);
	void SetLocalMatrix(const float3 &_pos, const Quat& _rot, const float3 &_scale);
	void SetGlobalMatrix(const math::float4x4 &matrix);

	void CalculeLocalMatrix();
	void CalculateGlobalMatrix();

	math::float4x4 GetLocalMatrix() const;
	math::float4x4 GetGlobalMatrix() const;

	math::float3 GetPosition() const;
	math::float3 GetGlobalPosition() const;
	math::Quat	 GetRotation() const;
	math::Quat	 GetGlobalRotation() const;
	math::float3 GetScale() const;

	void Save(nlohmann::json& node) override;
	void Load(const nlohmann::json& node) override;

private:
	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 euler_rot = math::float3::zero;
	math::float3 scale = math::float3::one;

	math::float4x4 local_matrix = math::float4x4::identity;
	math::float4x4 global_matrix = math::float4x4::identity;

	bool local_guizmo = false;

	//Lua functions glue code
private:
	void LSetPositionV(const float3& vector);
	void LSetPosition3f(const float& x, const float& y, const float& z);
	void LSetRotationQ(const Quat& quat);
	void LSetScale3f(const float& x, const float& y, const float& z);
	float3 LGetForward() const;
	float3 LGetUp() const;
	float3 LGetRight() const;

	void LLookAt(const float3& pos);

	GameObject* LGetParent() const;

	GameObject* Find(const char* n) const;
	int			ChildCount() const;
	GameObject* GetChild(const int& index) const;
	bool		IsChildOf(const ComponentTransform* parent) const;
	void		SetParent(const ComponentTransform* parent);
};
