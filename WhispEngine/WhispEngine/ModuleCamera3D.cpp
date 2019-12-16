#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float2.h"
#include "PanelScene.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Brofiler/Brofiler.h"

#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleObjectManager.h"

#include "MathGeoLib/include/Geometry/AABB.h"
ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name.assign("Camera3D");

	scene_camera = CreateCamera();
	scene_camera->SetFarZ(450.f);
	game_camera = CreateCamera();
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	sensiblity = 0.25f;  //  TODO: Save and Load this data in JSON
	movement_speed = 10;  //  TODO: Save and Load this data in JSON
	wheel_speed = 70;  //  TODO: Save and Load this data in JSON
	slowness_middle_mouse = 5;  //  TODO: Save and Load this data in JSON
	slowness_zoom_in_out = 50;  //  TODO: Save and Load this data in JSON

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	DeleteVectorCameras();

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	BROFILER_CATEGORY("Camera", Profiler::Color::Coral);

	if (scene_camera != nullptr)
	{
		float cam_speed = movement_speed * App->GetDeltaTime();
		float whe_speed = wheel_speed * App->GetDeltaTime();
		float mou_speed = sensiblity * App->GetDeltaTime();

		//  Triplicate Speed ----------------
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			cam_speed = movement_speed * 3 * App->GetDeltaTime();


		// Mouse OnHover scene rect ----------------
		static ImVec2 scene_position;
		static ImVec2 scene_size;

		scene_position = App->gui->scene->GetPanelPos();
		scene_size = App->gui->scene->GetPanelSize();
		scene_size = { scene_position.x + scene_size.x, scene_position.y + scene_size.y };

		//Makes posible keep moving camera out of panel if button is pressed
		MoveCameraOutScene(is_moving_camera);
		
		//Camera Functionalities
		if ((ImGui::IsMouseHoveringRect(scene_position, scene_size, false) || is_moving_camera) && App->gui->scene->active)
		{
			if (App->input->GetMouseWheel() >= 1)
			{
				scene_camera->Movement(CameraMovementType::FRONT, whe_speed);
			}
			else if (App->input->GetMouseWheel() <= -1)
			{
				scene_camera->Movement(CameraMovementType::BACK, whe_speed);
			}

			// Mouse Zoom in/out
			static math::float2 initial_mouse_position = math::float2::zero;
			static math::float2 last_mouse_position = math::float2::zero;

			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
				initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };

			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				last_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
				math::float2 mouse_vec = last_mouse_position - initial_mouse_position;

				if (App->input->GetMouseXMotion() > 0) scene_camera->Movement(CameraMovementType::BACK, mouse_vec.x / slowness_zoom_in_out);
				if (App->input->GetMouseYMotion() > 0) scene_camera->Movement(CameraMovementType::FRONT, mouse_vec.y / slowness_zoom_in_out);
				if (App->input->GetMouseXMotion() < 0) scene_camera->Movement(CameraMovementType::BACK, mouse_vec.x / slowness_zoom_in_out);
				if (App->input->GetMouseYMotion() < 0) scene_camera->Movement(CameraMovementType::FRONT, mouse_vec.y / slowness_zoom_in_out);

				initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
				is_moving_camera = true;
			}
			else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				if (App->object_manager->GetSelected() != nullptr)
				{
					GameObject* obj_selected = App->object_manager->GetSelected();
					math::float3 obj_position = float3::zero;
					
					ComponentMesh* mesh = nullptr;
					if (obj_selected->TryGetComponent(ComponentType::MESH, (Component*&)mesh)) {
						obj_position = mesh->GetAABB().CenterPoint();
					}
					else {
						obj_position = ((ComponentTransform*)obj_selected->GetComponent(ComponentType::TRANSFORM))->GetPosition();
					}

					math::float2 orbit_position(-App->input->GetMouseXMotion()*mou_speed, -App->input->GetMouseYMotion()*mou_speed);
					scene_camera->OrbitObject(obj_position, orbit_position);
					scene_camera->Look(obj_position);
					is_moving_camera = true;
				}
			}

			//Middle Mouse Button Movement
			if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN) {
				initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
			}
			if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
			{
				last_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
				math::float2 mouse_vec = last_mouse_position - initial_mouse_position;

				float3 to_move = scene_camera->frustum.WorldRight() * -mouse_vec.x + scene_camera->frustum.up * mouse_vec.y;
				scene_camera->frustum.Translate(to_move * App->GetDeltaTime());
				
				initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
				is_moving_camera = true;

				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
			}

			//Move Camera
			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !App->input->GetKey(SDL_SCANCODE_LALT))
			{
				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
					scene_camera->Movement(CameraMovementType::FRONT, cam_speed);

				if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
					scene_camera->Movement(CameraMovementType::BACK, cam_speed);

				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
					scene_camera->Movement(CameraMovementType::LEFT, cam_speed);

				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
					scene_camera->Movement(CameraMovementType::RIGHT, cam_speed);

				math::float2 rotation_position(-App->input->GetMouseXMotion()*mou_speed, -App->input->GetMouseYMotion()*mou_speed);
				scene_camera->CameraViewRotation(rotation_position);
				is_moving_camera = true;
			}

		}
		if (!ImGui::IsAnyItemActive() && App->object_manager->GetSelected() != nullptr) {
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
				scene_camera->is_focusing = true;

			scene_camera->FocusObject(App->object_manager->GetSelected()->GetAABB());
		}
	}


	//Check if exists any camera component
	std::vector<GameObject*> all_game_objs;
	App->object_manager->GetChildsFrom(App->object_manager->root, all_game_objs); // TODO: Iterate all gameobjects costs a lot of ms, we have to optimize that :)

	std::vector<GameObject*> obj_with_camera;
	for (GameObject* i : all_game_objs)
	{
		if ((ComponentCamera*)i->GetComponent(ComponentType::CAMERA) != nullptr)
		{
			obj_with_camera.push_back(i);
		}
	}
	if (obj_with_camera.size() == 1)
	{
		((ComponentCamera*)obj_with_camera[0]->GetComponent(ComponentType::CAMERA))->only_one_camera = true;
		((ComponentCamera*)obj_with_camera[0]->GetComponent(ComponentType::CAMERA))->checkbox_main_camera = true;
		((ComponentCamera*)obj_with_camera[0]->GetComponent(ComponentType::CAMERA))->is_main_camera = true;
	}
	else if(!obj_with_camera.empty())
		((ComponentCamera*)obj_with_camera[0]->GetComponent(ComponentType::CAMERA))->only_one_camera = false;

	obj_with_camera.clear();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
Camera * ModuleCamera3D::CreateCamera()
{
	Camera* cam = new Camera();
	cameras.push_back(cam);

	return cam;
}

void ModuleCamera3D::DeleteVectorCameras()
{
	for (std::vector<Camera*>::iterator camera = cameras.begin(); camera != cameras.end();)
	{
		RELEASE (*camera);
		camera = cameras.erase(camera);
	}
}

Camera * ModuleCamera3D::GetSceneCamera()
{
	return scene_camera;
}

Camera * ModuleCamera3D::GetGameCamera()
{
	return game_camera;
}

void ModuleCamera3D::SetGameCamera(Camera * camera)
{
	if (camera != nullptr)
	{
		std::vector<GameObject*> gameobjects;
		App->object_manager->GetChildsFrom(App->object_manager->root, gameobjects);

		for (GameObject* go : gameobjects)
		{
			ComponentCamera* camera_component = (ComponentCamera*)go->GetComponent(ComponentType::CAMERA);

			if (camera_component != nullptr)
			{
				camera_component->checkbox_main_camera = false;
			}
		}
		
		ComponentCamera* actual_camera_component = (ComponentCamera*)App->object_manager->GetSelected()->GetComponent(ComponentType::CAMERA);
		actual_camera_component->checkbox_main_camera = true;
		game_camera = camera;
		
	}
}

// -----------------------------------------------------------------

void ModuleCamera3D::MoveCameraOutScene(bool &is_moving)
{
	if(App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		is_moving = false;
}


Camera::Camera()
{
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = { 0, 1, -1 };
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	
	SetNearZ(1.f);
	SetFarZ(200.f);
	SetAspectRatio(1.f);
	SetFOV(70.f);

	//JSON
	offset_reference_focus = 5;
	focus_speed = 100;
}

void Camera::GetAllCorners(float3 * corners)
{
	frustum.GetCornerPoints(corners);
}

const float3 Camera::GetPosition()
{
	return frustum.pos;
}

void Camera::SetNearZ(const float &zNear)
{
	frustum.nearPlaneDistance = zNear;
}

void Camera::SetFarZ(const float &zFar)
{
	frustum.farPlaneDistance = zFar;
}

void Camera::SetFOV(const float &fov)
{
	frustum.verticalFov = DEGTORAD * fov;
	float _verticalFov = tanf(frustum.verticalFov/2.f);

	frustum.horizontalFov = 2.f * atanf(_verticalFov * aspect_ratio);
}

void Camera::SetAspectRatio(const float &ratio)
{
	aspect_ratio = ratio;

	if (frustum.horizontalFov > 0 && frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

void Camera::SetTransformPosition(const float3 &position)
{
	frustum.pos = position;
}

void Camera::SetVectorDirectionFront(const float3 &pos)
{
	frustum.front = pos.Normalized();
}

void Camera::SetVectorDirectionUp(const float3 &pos)
{
	frustum.up = pos.Normalized();
}

void Camera::Movement(CameraMovementType type, const float & speed)
{
	float3 displacement = float3::zero;

	switch (type)
	{
	case FRONT:
		displacement += frustum.front * speed;
		frustum.Translate(displacement);
		break;
	case BACK:
		displacement -= frustum.front * speed;
		frustum.Translate(displacement);
		break;
	case RIGHT:
		displacement += frustum.WorldRight() * speed;
		frustum.Translate(displacement);
		break;
	case LEFT:
		displacement -= frustum.WorldRight() * speed;
		frustum.Translate(displacement);
		break;
	case UP:
		displacement += float3::unitY * speed;
		frustum.Translate(displacement);
		break;
	case DOWN:
		displacement -= float3::unitY * speed;
		frustum.Translate(displacement);
		break;
	default:
		break;
	}
}

void Camera::OrbitObject(const float3& center, const float2& pos)
{
	math::float3 displacement = frustum.pos - center;
	math::Quat quat_rotation_x(frustum.WorldRight(), pos.y);
	math::Quat quat_rotation_y(frustum.up, pos.x);

	displacement = quat_rotation_x.Transform(displacement);
	displacement = quat_rotation_y.Transform(displacement);
	frustum.pos = (displacement + center);
}

void Camera::CameraViewRotation(const float2& pos)
{
	math::Quat quat_rotation_x = math::Quat::RotateY(pos.x);
	MulQuat(quat_rotation_x);

	math::Quat quat_rotation_y = math::Quat::RotateAxisAngle(frustum.WorldRight(), pos.y);
	 MulQuat(quat_rotation_y);
}

void Camera::MulQuat(math::Quat & quat)
{
	frustum.up = quat.Mul(frustum.up).Normalized();
	frustum.front = quat.Mul(frustum.front).Normalized();
}

void Camera::FocusObject(const AABB & aabb)
{
	static math::float3 actual_camera_position(0, 0, 0);
	static math::float3 reference_position(0, 0, 0);

	if (is_focusing)
	{
		GameObject* sel = App->object_manager->GetSelected();

		actual_camera_position = frustum.pos;
		math::float3 center = aabb.CenterPoint();
		ComponentTransform* trans = (ComponentTransform*)sel->GetComponent(ComponentType::TRANSFORM);

		if (aabb.IsFinite())
			reference_position = math::float3(center.x, center.y, center.z);
		else
			reference_position = trans->GetPosition();

		Look(reference_position);

		math::float3  diff = reference_position - actual_camera_position;
		diff = { abs(diff.x),
				 abs(diff.y),
				 abs(diff.z)
		};

		float mag_diff = sqrt((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));

		math::float3 new_pos = float3::zero;
		math::float3 vector = new_pos - reference_position;

		float object_length = vector.Normalized().Length() * aabb.Diagonal().Length();

		ComponentMesh* component_mesh = (ComponentMesh*)App->object_manager->GetSelected()->GetComponent(ComponentType::MESH);
		if (component_mesh == nullptr)
			object_length = (float)5;


		//  Move Camera 
		if (mag_diff >= object_length + 5)
		{
			Movement(CameraMovementType::FRONT, focus_speed * App->GetDeltaTime());
		}
		else
			this->is_focusing = false; //Stop Camera
	}
}

void Camera::Look(const float3 &look_pos)
{
	math::float3 dir = look_pos - frustum.pos;

	math::float3x3 direction_matrix = math::float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = direction_matrix.MulDir(frustum.front).Normalized();
	frustum.up = direction_matrix.MulDir(frustum.up).Normalized();
}


const float Camera::GetNearZ() const
{
	return frustum.nearPlaneDistance;
}

const float Camera::GetFarZ() const
{
	return frustum.farPlaneDistance;
}

const float Camera::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

const float4x4 Camera::GetViewMatrix() const
{
	float4x4 view = frustum.ViewMatrix();
	return view.Transposed();
}

const float4x4 Camera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

const float Camera::GetAspectRatio() const
{
	return aspect_ratio;
}


void Camera::DrawInsideFrustum()
{
	//Keep all game objects of the scene
	std::vector<GameObject*> game_objects;
	App->object_manager->GetChildsFrom(App->object_manager->root, game_objects);

	for (GameObject* go : game_objects)
	{
		if (go->GetAABB().IsFinite())
		{
			if (BboxIntersectsFrustum(go->GetAABB()))
				go->is_inside_frustum = true;			
			else
				go->is_inside_frustum = false;
		}		
	}
}

bool Camera::BboxIntersectsFrustum(const AABB & box)
{
	static int planes = 6;
	static int vertex = 8;

	math::float3 pos_vertex[8];
	
	box.GetCornerPoints(pos_vertex);

	for (auto i = 0; i < planes; ++i)
	{
		int vertex_inside = vertex;

		for (auto j = 0; j < vertex; ++j)
			if (App->camera->GetGameCamera()->frustum.GetPlane(i).IsOnPositiveSide(pos_vertex[j]))
				vertex_inside--;		

		if (vertex_inside < 1)
			return false;
	}

	return true;
}

Frustum Camera::GetFrustum()
{
	return frustum;
}
