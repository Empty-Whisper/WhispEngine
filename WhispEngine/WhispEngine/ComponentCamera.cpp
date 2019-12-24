#include "ComponentCamera.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/float3.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "PanelScene.h"
#include "ModuleObjectManager.h"
#include "Imgui/imgui_internal.h"


ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent, ComponentType::CAMERA)
{
	camera = App->camera->CreateCamera();
}

ComponentCamera::~ComponentCamera()
{}

void ComponentCamera::Update()
{
	float3 corners[8];
	camera->GetAllCorners(corners);

	//Update Component Transform
	camera->SetTransformPosition(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetPosition());
	camera->SetVectorDirectionFront(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().WorldZ());
	camera->SetVectorDirectionUp(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().WorldY());
	
	//Log Drawing
	DrawFrustum();

}

void ComponentCamera::OnInspector()
{
	ActiveImGui();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {

		float fov = camera->GetVerticalFOV();
		float zNear = camera->GetNearZ();
		float zFar = camera->GetFarZ();

		if (ImGui::DragFloat("Field of View (FOV)", (float*)&fov, 0.1f, 0.1f, 179))
			camera->SetFOV(fov);

		if (ImGui::DragFloat("zNear", (float*)&zNear, 0.1f, 1, zFar))
			camera->SetNearZ(zNear);

		if (ImGui::DragFloat("zFar", (float*)&zFar, 0.1f, zNear, 1000))
			camera->SetFarZ(zFar);

		ImGui::Separator();

		ImGui::Checkbox("Camera Preview", &App->gui->scene->preview_checkbox);
		
		ImGui::Checkbox("Frustum Culling", &App->camera->activate_frustum_culling);

		ImGui::Separator();

		// Enable/Disable main camera checkbox if theres more than one =============================
		if (only_one_camera) 
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Checkbox("Main Camera", &checkbox_main_camera))
			is_main_camera = true;
		if (checkbox_main_camera && is_main_camera)
		{
			App->camera->SetGameCamera(camera);
			is_main_camera = false;
		}

		if (only_one_camera) 
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		// =================================================================================

	}
}

void ComponentCamera::DrawFrustum()
{
	float3 vertices[8];
	camera->GetFrustum().GetCornerPoints(vertices);
	glColor3f(1.f, 1.f, 1.f);

	glBegin(GL_LINES);

	glVertex3fv((float*)&vertices[0]);
	glVertex3fv((float*)&vertices[4]);

	glVertex3fv((float*)&vertices[0]);
	glVertex3fv((float*)&vertices[1]);

	glVertex3fv((float*)&vertices[0]);
	glVertex3fv((float*)&vertices[2]);

	glVertex3fv((float*)&vertices[5]);
	glVertex3fv((float*)&vertices[4]);

	glVertex3fv((float*)&vertices[5]);
	glVertex3fv((float*)&vertices[1]);

	glVertex3fv((float*)&vertices[7]);
	glVertex3fv((float*)&vertices[5]);

	glVertex3fv((float*)&vertices[7]);
	glVertex3fv((float*)&vertices[3]);

	glVertex3fv((float*)&vertices[7]);
	glVertex3fv((float*)&vertices[6]);

	glVertex3fv((float*)&vertices[2]);
	glVertex3fv((float*)&vertices[6]);

	glVertex3fv((float*)&vertices[2]);
	glVertex3fv((float*)&vertices[3]);

	glVertex3fv((float*)&vertices[1]);
	glVertex3fv((float*)&vertices[3]);

	glVertex3fv((float*)&vertices[4]);
	glVertex3fv((float*)&vertices[6]);

	glEnd();
	glLineWidth(1.f);

	glColor3f(1.f, 1.f, 1.f);

}

void ComponentCamera::Save(nlohmann::json & node)
{
	node["is_main_camera"] = is_main_camera;
	node["checkbox_main_camera"] = checkbox_main_camera; // TODO: delete this bool

	node["aspect_ratio"] =	camera->GetAspectRatio();
	node["vertical_fov"] =	camera->GetVerticalFOV();
	node["far_plane"] =		camera->GetFarZ();
	node["near_plane"] =    camera->GetNearZ();
}

void ComponentCamera::Load(const nlohmann::json & node)
{
	is_main_camera = node["is_main_camera"];
	checkbox_main_camera = node["checkbox_main_camera"];

	camera->SetAspectRatio(node["aspect_ratio"]);
	camera->SetFOV(node["vertical_fov"]);
	camera->SetFarZ(node["far_plane"]);
	camera->SetNearZ(node["near_plane"]);
}
