#include "ComponentTransform.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/MathFunc.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent, ComponentType::TRANSFORM)
{
}


ComponentTransform::~ComponentTransform()
{
	
}

void ComponentTransform::PreUpdate()
{

}

void ComponentTransform::OnInspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::InputFloat3("Position", position.ptr(), 3)) {
			CalculateLocalMatrix();
		}
		math::float3 to_rot = RadToDeg(rotation.ToEulerXYZ());
		if (ImGui::SliderFloat3("Rotation", to_rot.ptr(), -360, 360)) {
			to_rot = DegToRad(to_rot*0.5f);
			rotation.Set(math::float4x4::FromEulerXYZ(to_rot.x, to_rot.y, to_rot.z));
			CalculateLocalMatrix();
		}
		if (ImGui::InputFloat3("Scale", scale.ptr(), 3)) {
			CalculateLocalMatrix();
		}
	}
}

void ComponentTransform::SetPosition(const float & x, const float & y, const float & z)
{
	position.Set(x, y, z);
}

void ComponentTransform::SetRotation(const float & x, const float & y, const float & z, const float & w)
{
	rotation.Set(x, y, z, w);
}

void ComponentTransform::SetScale(const float & x, const float & y, const float & z)
{
	scale.Set(x, y, z);
}

void ComponentTransform::CalculateLocalMatrix()
{
	local_matrix = math::float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::CalculateGlobalMatrix()
{
	if (parent != nullptr) {
		global_matrix = local_matrix;
		global_matrix = ((ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM))->global_matrix * local_matrix;
	}
}

math::float4x4 ComponentTransform::GetLocalMatrix() const
{
	return local_matrix;
}

math::float4x4 ComponentTransform::GetGlobalMatrix() const
{	
	return global_matrix;
}

const float* ComponentTransform::GetPtrGlobalMatrix() const
{
	return global_matrix.ptr();
}
