#include "ComponentTransform.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "Application.h"
#include "MathGeoLib/include/Math/MathFunc.h"
#include "Imgui/ImGuizmo.h"
#include "ComponentCamera.h"
#include "ModuleGUI.h"
#include "ModuleObjectManager.h"
#include "Imgui/imgui_internal.h"
#include "ModuleScripting.h"
#include "ModuleCamera3D.h"
#include "Lua/LuaBridge/LuaBridge.h"

#include "mmgr/mmgr.h"

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
		if (object->IsStatic()) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

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

		if (object->IsStatic()) 
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

void ComponentTransform::SetPosition(const float & x, const float & y, const float & z)
{
	position.Set(x, y, z);
	CalculeLocalMatrix();
}

void ComponentTransform::SetPosition(const float3& pos)
{
	position = pos;
	CalculeLocalMatrix();
}

void ComponentTransform::SetRotation(const float & w, const float & x, const float & y, const float & z)
{
	rotation.Set(x, y, z, w);
	CalculeLocalMatrix();
}

void ComponentTransform::SetRotation(const Quat &rot)
{
	rotation = rot;
	CalculeLocalMatrix();
}

void ComponentTransform::SetScale(const float & x, const float & y, const float & z)
{
	scale.Set(x, y, z);
	CalculeLocalMatrix();
}

void ComponentTransform::SetScale(const float3& _scale)
{
	scale = _scale;
	CalculeLocalMatrix();
}

void ComponentTransform::SetLocalMatrix(const math::float4x4 & matrix)
{
	local_matrix = matrix;
	local_matrix.Decompose(position, rotation, scale);
	euler_rot = math::RadToDeg(rotation.ToEulerXYZ());
}

void ComponentTransform::SetLocalMatrix(const float3 &_pos, const Quat& _rot, const float3 &_scale)
{
	position = _pos;
	rotation = _rot;
	euler_rot = math::RadToDeg(rotation.ToEulerXYZ());
	scale	 = _scale;
	local_matrix = float4x4::FromTRS(position, rotation, scale);
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
}

void ComponentTransform::CalculateGlobalMatrix()
{
	local_matrix = math::float4x4::FromTRS(position, rotation, scale);
	global_matrix = local_matrix;
	if (object->parent != nullptr) {
		global_matrix = ((ComponentTransform*)object->parent->GetComponent(ComponentType::TRANSFORM))->global_matrix * local_matrix;
	}

	ComponentMesh* mesh = (ComponentMesh*)object->GetComponent(ComponentType::MESH);
	if (mesh != nullptr)
		mesh->CalulateAABB_OBB();

	ComponentCamera* cam = nullptr;
	if (object->TryGetComponent(ComponentType::CAMERA, (Component*&)cam)) {
		float3 pos, scale;
		Quat rot;
		global_matrix.Decompose(pos, rot, scale);
		
		cam->camera->SetTransformPosition(pos); //TODO fix camera does not rotate around parent
		cam->camera->SetVectorDirectionFront(rotation.WorldZ());
		cam->camera->SetVectorDirectionUp(rotation.WorldY());
		/*cam->camera->SetTransformPosition(position);
		cam->camera->SetVectorDirectionFront(rotation.WorldZ());
		cam->camera->SetVectorDirectionUp(rotation.WorldY());*/
	}

	for (auto i = object->children.begin(); i != object->children.end(); i++) {
		((ComponentTransform*)(*i)->GetComponent(ComponentType::TRANSFORM))->CalculateGlobalMatrix();
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

math::float3 ComponentTransform::GetGlobalPosition() const
{
	float3 pos, scale;
	Quat rot;
	global_matrix.Decompose(pos, rot, scale);
	return pos;
}

math::Quat ComponentTransform::GetRotation() const
{
	return rotation;
}

math::Quat ComponentTransform::GetGlobalRotation() const
{
	float3 pos, scale;
	Quat rot;
	global_matrix.Decompose(pos, rot, scale);
	return rot;
}

math::float3 ComponentTransform::GetScale() const
{
	return scale;
}

void ComponentTransform::Save(nlohmann::json & node)
{
	App->json->AddFloat3("position", position, node);
	App->json->AddQuaternion("rotation", rotation, node);
	App->json->AddFloat3("scale", scale, node);
}

void ComponentTransform::Load(const nlohmann::json & node)
{
	position = App->json->GetFloat3("position", node);
	rotation = App->json->GetQuaternion("rotation", node);
	scale    = App->json->GetFloat3("scale", node);
	CalculeLocalMatrix();
}

void ComponentTransform::LSetPositionV(const float3 & vector)
{
	position = vector;
	CalculeLocalMatrix();
}

void ComponentTransform::LSetPosition3f(const float & x, const float & y, const float & z)
{
	position.Set(x, y, z);
	CalculeLocalMatrix();
}

void ComponentTransform::LSetRotationQ(const Quat & quat)
{
	rotation = quat;
	euler_rot = rotation.ToEulerXYZ();
	CalculeLocalMatrix();
}

void ComponentTransform::LSetScale3f(const float & x, const float & y, const float & z)
{
	scale.Set(x, y, z);
	CalculeLocalMatrix();
}

float3 ComponentTransform::LGetForward() const
{
	return rotation.WorldZ();
}

float3 ComponentTransform::LGetUp() const
{
	return rotation.WorldY();
}

float3 ComponentTransform::LGetRight() const
{
	return rotation.WorldX();
}

void ComponentTransform::LLookAt(const float3 & pos)
{
	rotation = Quat::LookAt(-rotation.WorldZ(), pos, rotation.WorldY(), float3::unitY);
	euler_rot = rotation.ToEulerXYZ();
	CalculateGlobalMatrix();
}

GameObject * ComponentTransform::LGetParent() const
{
	return object->parent;
}

GameObject * ComponentTransform::Find(const char * n) const
{
	for (auto i = object->children.begin(); i != object->children.end(); i++) {
		if (strcmp((*i)->GetName(), n) == 0)
			return *i;
	}
	LOG("GameObject: %s not found in %s childs", n, object->GetName());
	return nullptr;
}

int ComponentTransform::ChildCount() const
{
	return object->children.size();
}

GameObject * ComponentTransform::GetChild(const int & index) const
{
	if (index >= object->children.size()) {
		LOG("Failed to GetChild: index given is greater than children size");
		return nullptr;
	}

	return object->children[index];
}

bool ComponentTransform::IsChildOf(const ComponentTransform* parent) const
{
	for (auto i = parent->object->children.begin(); i != parent->object->children.end(); i++) {
		if (*i == object)
			return true;
	}
	return false;
}

void ComponentTransform::SetParent(const ComponentTransform* parent)
{
	if (object != parent->object)
		App->object_manager->ChangeHierarchy(object, parent->object);
}
