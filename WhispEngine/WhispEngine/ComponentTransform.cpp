#include "ComponentTransform.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/MathFunc.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent, ComponentType::TRANSFORM)
{
	CalculeLocalMatrix();
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
			CalculeLocalMatrix();
		}
		math::float3 to_rot = (rotation.ToEulerXYZ());
		if (ImGui::SliderFloat3("Rotation", to_rot.ptr(), -PI, PI)) {
			rotation.Set(math::float4x4::FromEulerXYZ(to_rot.x, to_rot.y, to_rot.z));
			CalculeLocalMatrix();
		}
		if (ImGui::InputFloat3("Scale", scale.ptr(), 3)) {
			CalculeLocalMatrix();
		}
	}
}

void ComponentTransform::SetPosition(const float & x, const float & y, const float & z)
{
	position.Set(x, y, z);
}

void ComponentTransform::SetRotation(const float & w, const float & x, const float & y, const float & z)
{
	rotation.Set(x, y, z, w);
}

void ComponentTransform::SetScale(const float & x, const float & y, const float & z)
{
	scale.Set(x, y, z);
}

void ComponentTransform::SetLocalMatrix(const math::float4x4 & matrix)
{
	local_matrix = matrix;
	local_matrix.Decompose(position, rotation, scale);
}

void ComponentTransform::CalculeLocalMatrix()
{
	local_matrix = math::float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::CalculateGlobalMatrix()
{
	global_matrix = local_matrix;
	if (parent != nullptr) {
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
