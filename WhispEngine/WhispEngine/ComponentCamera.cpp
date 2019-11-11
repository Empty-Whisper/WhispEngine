#include "ComponentCamera.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Application.h"


ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent, ComponentType::CAMERA)
{
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update()
{
	DrawFrustum();
	CalculateZNear(f_depth);
	CalculateZFar(f_initial_z);
	
}

void ComponentCamera::OnInspector()
{
	ActiveImGui();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::SliderFloat("Field of View (FOV)", (float*)&f_fov, 0, 3);
		ImGui::SliderFloat("zNear", (float*)&f_depth, 1, 50);
		ImGui::SliderFloat("zFar", (float*)&f_initial_z, 1, 50);
	}
}

void ComponentCamera::DrawFrustum()
{
	zFar.up_right = { f_center.x + zFar.width*0.5f, f_center.y + zFar.height*0.5f, f_center.z + f_initial_z };
	zFar.up_left = { f_center.x - zFar.width*0.5f, f_center.y + zFar.height*0.5f, f_center.z + f_initial_z };
	zFar.down_right = { f_center.x + zFar.width*0.5f, f_center.y - zFar.height*0.5f, f_center.z + f_initial_z };
	zFar.down_left = { f_center.x - zFar.width*0.5f, f_center.y - zFar.height*0.5f, f_center.z + f_initial_z };

	zNear.up_right = { f_center.x + zNear.width*0.5f, f_center.y + zNear.height*0.5f, f_center.z + f_depth };
	zNear.up_left = { f_center.x - zNear.width*0.5f, f_center.y + zNear.height*0.5f, f_center.z + f_depth };
	zNear.down_right = { f_center.x + zNear.width*0.5f, f_center.y - zNear.height*0.5f, f_center.z + f_depth };
	zNear.down_left = { f_center.x - zNear.width*0.5f, f_center.y - zNear.height*0.5f, f_center.z + f_depth };

	glDisable(GL_LIGHTING);
	glColor3f(0.f, 0.f, 1.f);

	glBegin(GL_LINES);

	//zFar
	glVertex3f(zFar.down_left.x, zFar.down_left.y, zFar.down_left.z);
	glVertex3f(zFar.up_left.x, zFar.up_left.y, zFar.up_left.z);

	glVertex3f(zFar.up_left.x, zFar.up_left.y, zFar.up_left.z);
	glVertex3f(zFar.up_right.x, zFar.up_right.y, zFar.up_right.z);

	glVertex3f(zFar.up_right.x, zFar.up_right.y, zFar.up_right.z);
	glVertex3f(zFar.down_right.x, zFar.down_right.y, zFar.down_right.z);

	glVertex3f(zFar.down_right.x, zFar.down_right.y, zFar.down_right.z);
	glVertex3f(zFar.down_left.x, zFar.down_left.y, zFar.down_left.z);

	//zNear
	glVertex3f(zNear.down_left.x, zNear.down_left.y, zNear.down_left.z);
	glVertex3f(zNear.up_left.x, zNear.up_left.y, zNear.up_left.z);

	glVertex3f(zNear.up_left.x, zNear.up_left.y, zNear.up_left.z);
	glVertex3f(zNear.up_right.x, zNear.up_right.y, zNear.up_right.z);

	glVertex3f(zNear.up_right.x, zNear.up_right.y, zNear.up_right.z);
	glVertex3f(zNear.down_right.x, zNear.down_right.y, zNear.down_right.z);

	glVertex3f(zNear.down_right.x, zNear.down_right.y, zNear.down_right.z);
	glVertex3f(zNear.down_left.x, zNear.down_left.y, zNear.down_left.z);

	//zConection
	glVertex3f(zFar.down_left.x, zFar.down_left.y, zFar.down_left.z);
	glVertex3f(zNear.down_left.x, zNear.down_left.y, zNear.down_left.z);

	glVertex3f(zFar.up_left.x, zFar.up_left.y, zFar.up_left.z);
	glVertex3f(zNear.up_left.x, zNear.up_left.y, zNear.up_left.z);

	glVertex3f(zFar.up_right.x, zFar.up_right.y, zFar.up_right.z);
	glVertex3f(zNear.up_right.x, zNear.up_right.y, zNear.up_right.z);

	glVertex3f(zFar.down_right.x, zFar.down_right.y, zFar.down_right.z);
	glVertex3f(zNear.down_right.x, zNear.down_right.y, zNear.down_right.z);


	glEnable(GL_LIGHTING);

	glEnd();
}

void ComponentCamera::CalculateZNear(const float f_near)
{
	zNear.height = 2 * tan(f_fov / 2) * f_near;
	zNear.width = zNear.height * 1;
}

void ComponentCamera::CalculateZFar(const float f_far)
{
	zFar.height = 2 * tan(f_fov / 2) * f_far;
	zFar.width = zFar.height * 1;
}
