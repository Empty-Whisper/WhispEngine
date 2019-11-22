#include "ComponentTransform.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "Application.h"
#include "MathGeoLib/include/Math/MathFunc.h"
#include "Imgui/ImGuizmo.h"

#include "ModuleObjectManager.h"

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
		ImGui::PushID("POSITION");
		ImGui::Text("Position"); ImGui::SameLine(); App->gui->HelpMarker("(?)", "Double Click to turn drag box into an input box");

		ImGui::SetNextItemWidth(75.f);
		bool change_pos = ImGui::DragFloat("X", &position.x, 0.2f, -50.f, 50.f); //In different lines ('|=') because if just || did not print the next sliders
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change_pos		|= ImGui::DragFloat("Y", &position.y, 0.2f, -50.f, 50.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change_pos		|= ImGui::DragFloat("Z", &position.z, 0.2f, -50.f, 50.f);

		if (change_pos) {
			CalculeLocalMatrix();
		}
		ImGui::PopID();

		ImGui::Separator();

		ImGui::PushID("ROTATION");
		ImGui::Text("Rotation");

		ImGui::SetNextItemWidth(75.f);
		bool change_rot = ImGui::DragFloat("X", &euler_rot.x, 1.f, -360.f, 360.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change_rot		|= ImGui::DragFloat("Y", &euler_rot.y, 1.f, -360.f, 360.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change_rot		|= ImGui::DragFloat("Z", &euler_rot.z, 1.f, -360.f, 360.f);

		if(change_rot){
			float3 to_rot = math::DegToRad(euler_rot);
			rotation = math::Quat::FromEulerXYZ(to_rot.x, to_rot.y, to_rot.z);
			CalculeLocalMatrix();
		}
		ImGui::PopID();

		ImGui::Separator();

		ImGui::PushID("SCALE");
		ImGui::Text("Scale");

		ImGui::SetNextItemWidth(75);
		bool change_scale	 = ImGui::DragFloat("X", &scale.x, 0.5f, 0.f, 100.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change_scale		|= ImGui::DragFloat("Y", &scale.y, 0.5f, 0.f, 100.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change_scale		|= ImGui::DragFloat("Z", &scale.z, 0.5f, 0.f, 100.f);
		if (change_scale) {
			CalculeLocalMatrix();
		}
		ImGui::PopID();


		//Guizmo
		ImGuizmo::MODE guizmoLocal = ImGuizmo::MODE::LOCAL;
		ImGuizmo::MODE guizmoWorld = ImGuizmo::MODE::WORLD;
		bool world_guizmo = !local_guizmo;

		ImGui::Separator();
		if (ImGui::Checkbox("Local", &local_guizmo))
			App->object_manager->ChangeGuizmoMode(guizmoLocal);

		ImGui::SameLine();

		if (ImGui::Checkbox("Global", &world_guizmo))
		{
			local_guizmo = !local_guizmo;
			App->object_manager->ChangeGuizmoMode(guizmoWorld);
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

void ComponentTransform::SetGlobalMatrix(const math::float4x4 & matrix)
{
	global_matrix = matrix;

	if (object->parent != nullptr)
	{
		float4x4 _local_matrix = ((ComponentTransform*)object->parent->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().Inverted() * global_matrix;
		SetLocalMatrix(_local_matrix);
	}
}


void ComponentTransform::CalculeLocalMatrix()
{
	local_matrix = math::float4x4::FromTRS(position, rotation, scale);
	CalculateGlobalMatrix();
	for (auto i = object->children.begin(); i != object->children.end(); i++) {
		((ComponentTransform*)(*i)->GetComponent(ComponentType::TRANSFORM))->CalculateGlobalMatrix();
	}
}

void ComponentTransform::CalculateGlobalMatrix()
{
	global_matrix = local_matrix;
	if (object->parent != nullptr) {
		global_matrix = ((ComponentTransform*)object->parent->GetComponent(ComponentType::TRANSFORM))->global_matrix * local_matrix;
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

math::float3 ComponentTransform::GetPosition() const
{
	return position;
}
