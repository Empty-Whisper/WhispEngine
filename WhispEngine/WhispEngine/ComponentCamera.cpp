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


ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent, ComponentType::CAMERA)
{
	camera = App->camera->CreateCamera();
}

ComponentCamera::~ComponentCamera()
{
	App->camera->DeleteCamera(camera);
}

void ComponentCamera::Update()
{
	float3 corners[8];
	camera->GetAllCorners(corners);

	//Update Component Transform
	camera->SetTransformPosition(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetPosition());
	camera->SetVectorDirectionFront(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().WorldZ());
	camera->SetVectorDirectionUp(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().WorldY());
	
	//Debug Drawing
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

		if (ImGui::Checkbox("Main Camera", &is_main_camera)) {
			if(is_main_camera)
			App->camera->SetGameCamera(camera);
		}

	}
}

void ComponentCamera::DrawFrustum()
{
	float3 vertices[8];
	camera->GetFrustum().GetCornerPoints(vertices);

	GLint previous[2];
	
	
	glGetIntegerv(GL_POLYGON_MODE, previous);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	glColor3f(0, 0, 100);

	glLineWidth(1.f);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

	
	glPolygonMode(GL_FRONT_AND_BACK, previous[0]);

	glLineWidth(1.0f);

	glColor3f(255, 255, 255);

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
