#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float2.h"
#include "PanelScene.h"
#include "ComponentTransform.h"
#include "Brofiler/Brofiler.h"
ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name.assign("Camera3D");

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 0.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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
	focus_speed = 100;  //  TODO: Save and Load this data in JSON
	wheel_speed = 70;  //  TODO: Save and Load this data in JSON
	offset_reference = 5;  //  TODO: Save and Load this data in JSON
	slowness_middle_mouse = 50;  //  TODO: Save and Load this data in JSON
	slowness_zoom_in_out = 50;  //  TODO: Save and Load this data in JSON

	f_initial_z = 0;
	f_depth = 6;
	f_fov = 0.5f;
	f_aspect = 1;
	
	zFar.width = 10;
	zFar.height = 10;
	zNear.width = 5;
	zNear.height = 5;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	BROFILER_CATEGORY("Camera", Profiler::Color::Coral);

	//Frustum
	DrawFrustum();
	CalculateZNear(f_depth);
	CalculateZFar(f_initial_z);
	frustum.ProjectionMatrix();
	frustum.ViewMatrix();
	//Camera
	vec3 newPos(0, 0, 0);
	float speed = movement_speed * App->GetDeltaTime();

	//  Triplicate Speed ----------------
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = movement_speed * 3 * App->GetDeltaTime();

	// Mouse OnHover scene rect ----------------
	static ImVec2 scene_position;
	static ImVec2 scene_size;

	scene_position = App->gui->scene->GetPanelPos();
	scene_size = App->gui->scene->GetPanelSize();
	scene_size = { scene_position.x + scene_size.x, scene_position.y + scene_size.y };

	ResetIsMovingCamera();

	if ((ImGui::IsMouseHoveringRect(scene_position, scene_size, false) || is_moving_camera) && App->gui->scene->active)
	{
		// Mouse Zoom in/out with wheel ----------------
		int mouse_wheel = App->input->GetMouseZ();

		if (mouse_wheel != 0)
			newPos -= mouse_wheel * Z * wheel_speed * App->GetDeltaTime();

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------	
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && !App->input->GetKey(SDL_SCANCODE_LALT))
		{
			MoveCameraByMouse(newPos, speed);
			is_moving_camera = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			MoveCameraOffsetByMouse(newPos, speed);
			is_moving_camera = true;
		}

		// Mouse Zoom in/out
		static math::float2 initial_mouse_position = initial_mouse_position.zero;
		static math::float2 last_mouse_position = last_mouse_position.zero;

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			last_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
			math::float2 mouse_vec = last_mouse_position - initial_mouse_position;

			if (App->input->GetMouseXMotion() > 0) newPos += Z * mouse_vec.x / slowness_zoom_in_out;
			if (App->input->GetMouseXMotion() < 0) newPos += Z * mouse_vec.x / slowness_zoom_in_out;
			if (App->input->GetMouseYMotion() < 0) newPos -= Z * mouse_vec.y / slowness_zoom_in_out;
			if (App->input->GetMouseYMotion() > 0) newPos -= Z * mouse_vec.y / slowness_zoom_in_out;

			Position += newPos;
			Reference += newPos;
			initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
			is_moving_camera = true;
		}

		// Middle Mouse Button Movement
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN) {
			initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
		}
			
		
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{

			last_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
			math::float2 mouse_vec = last_mouse_position - initial_mouse_position;

			if (App->input->GetMouseXMotion() > 0) newPos -= X * mouse_vec.x / slowness_middle_mouse;
			if (App->input->GetMouseXMotion() < 0) newPos -= X * mouse_vec.x / slowness_middle_mouse;
			if (App->input->GetMouseYMotion() < 0) newPos += Y * mouse_vec.y / slowness_middle_mouse;
			if (App->input->GetMouseYMotion() > 0) newPos += Y * mouse_vec.y / slowness_middle_mouse;

			Position += newPos;
			Reference += newPos;
			initial_mouse_position = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
			is_moving_camera = true;

			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
		}

	}
	if (!ImGui::IsAnyItemActive()) {
		// Focus Object
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->object_manager->GetSelected())
				is_focusing = true;
		}

		if (App->object_manager->GetSelected())
			FocusObject(newPos);
	}
	
	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::FocusObject(vec3 newPos)
{
	static vec3 actual_camera_position(0, 0, 0);
	static vec3 reference_position(0, 0, 0);

	if (is_focusing)
	{
		GameObject* sel = App->object_manager->GetSelected();
		AABB aabb = AABB(-float3::one, float3::one);

		if (sel != nullptr)
			aabb = ((ComponentMesh*)sel->GetComponent(ComponentType::MESH))->GetAABB();
		
		actual_camera_position = Position;
		float3 center = aabb.CenterPoint();
		reference_position = vec3(center.x, center.y, center.z);

		LookAt(reference_position);

		vec3 diff = reference_position - actual_camera_position;
		diff = { abs(diff.x),
				 abs(diff.y),
				 abs(diff.z)
		};

		float mag_diff = sqrt((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));

		vec3 vector = newPos - reference_position;
		
		float object_length = length(normalize(vector) * aabb.Diagonal().Length());

		ComponentMesh* component_mesh = (ComponentMesh*)App->object_manager->GetSelected()->GetComponent(ComponentType::MESH);
		if (component_mesh == nullptr)
			object_length = offset_reference;

		

		if (mag_diff >= object_length + offset_reference)
		{
			newPos -= Z * focus_speed * App->GetDeltaTime();
		}
		else
			this->is_focusing = false;


		Position += newPos;
		Reference += newPos;
	}
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::MoveCameraByMouse(vec3 newPos, float speed)
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference = Position;
	Position -= Reference;

	Position = Reference + CalculateMouseMotion() * length(Position);
}

void ModuleCamera3D::MoveCameraOffsetByMouse(vec3 newPos, float speed)
{
	GameObject* sel = App->object_manager->GetSelected();
	if (sel != nullptr) {
		float3 center = ((ComponentMesh*)sel->GetComponent(ComponentType::MESH))->GetAABB().CenterPoint(); //Get GameObject selected position
		Reference = vec3(center.x, center.y, center.z);
	}
	else
		Reference = vec3(0, 0, 0);
	Position += newPos;
	Reference += newPos;
	Position -= Reference;

	Position = Reference + CalculateMouseMotion() * length(Position);
}


// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}


void ModuleCamera3D::DrawFrustum()
{
	
	zFar.up_right = { f_center.x + zFar.width*0.5f, f_center.y + zFar.height*0.5f, f_center.z + f_initial_z};
	zFar.up_left = { f_center.x - zFar.width*0.5f, f_center.y + zFar.height*0.5f, f_center.z + f_initial_z};
	zFar.down_right = { f_center.x + zFar.width*0.5f, f_center.y - zFar.height*0.5f, f_center.z + f_initial_z};
	zFar.down_left = { f_center.x - zFar.width*0.5f, f_center.y - zFar.height*0.5f, f_center.z + f_initial_z};

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

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

vec3 ModuleCamera3D::CalculateMouseMotion()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	if (dx != 0)
	{
		float DeltaX = (float)dx * sensiblity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * sensiblity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	return Z;
}

void ModuleCamera3D::ResetIsMovingCamera()
{
	if(App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		is_moving_camera = false;
}

const vec3 ModuleCamera3D::GetTransformPosition()
{
	float3 obj_pos = ((ComponentTransform*)App->object_manager->GetSelected()->GetComponent(ComponentType::TRANSFORM))->GetPosition();
	return vec3(obj_pos.x,obj_pos.y,obj_pos.z); //TODO set all vec3 to math::float3
}

void ModuleCamera3D::CalculateZNear(const float f_near)
{
	zNear.height = 2 * tan(f_fov / 2) * f_near;
	zNear.width = zNear.height * 1;
}
void ModuleCamera3D::CalculateZFar(const float f_far)
{
	zFar.height = 2 * tan(f_fov / 2) * f_far;
	zFar.width = zFar.height * 1;
}

void ModuleCamera3D::CalculateAspect(const float aspect)
{
	zFar.width = aspect * zFar.height;
	zNear.width = aspect * zNear.height;
}
