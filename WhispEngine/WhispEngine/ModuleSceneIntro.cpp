#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "ModuleAudio.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	// Floor
	Cube* c = new Cube(3000, -1, 3000);
	c->color.Set(125, 125, 125, 0.25F);
	map.PushBack(c);

	//Map creation
	Cube road(1, 15, 1);
	road.color.Set(0.0f, 0.0f, 0.0f);
	int road_width = 15;
	int radius = 20;

	CreateRect(-7.0f, 0, -25, road_width, 125, road, ORIENTATION::NORTH);
	CreateCurve(-27.0f, 0, 100, road_width, 0, 90, road, radius);
	CreateRect(-25, 0, 120, road_width, 150, road, ORIENTATION::EAST);
	CreateCurve(-175.0f, 0.0f, 100, road_width, 90, 270, road, radius);
	CreateCurve(-175.0f, 0.0f, 45, road_width, 0, 90, road, radius);
	CreateRect(-155, 0, 43, road_width, 300, road,ORIENTATION::SOUTH);
	CreateCurve(-120, 0, -257, road_width, 180, 270, road, radius);
	CreateRect(-117, 0, -292, road_width, 150, road, ORIENTATION::WEST);
	CreateCurve(33, 0, -312, road_width, 0, 90, road, radius);
	CreateRect(53, 0, -314, road_width, 150, road, ORIENTATION::SOUTH);
	CreateCurve(88, 0, -465, road_width, 180, 360, road, radius);
	CreateRect(108, 0, -462, road_width, 200, road, ORIENTATION::NORTH);
	CreateCurve(88, 0, -262, road_width, 0, 90, road, radius);
	CreateCurve(86, 0, -207, road_width, 90, 270, road, radius);
	CreateRect(88, 0, -187, road_width, 150, road, ORIENTATION::WEST);
	CreateCurve(238, 0, -152, road_width, -90, 90, road, radius);
	CreateRect(235, 0, -132, road_width, 206, road, ORIENTATION::EAST);
	CreateCurve(29, 0, -152, road_width, 90, 180, road, radius);
	CreateRect(-6, 0, -154, road_width, 30, road, ORIENTATION::SOUTH);
	CreateCurve(-26, 0, -184, road_width, 180, 360, road, radius);
	CreateRect(-61, 0, -182, road_width, 100, road, ORIENTATION::NORTH);
	CreateCurve(-26, 0, -82, road_width, 95, 180, road, radius);
	CreateCurve(-27, 0, -27, road_width, -90, 0, road, radius);

	//Pendulums
	CreatePendulum(-41, 129);
	CreatePendulum(-79, 124);
	CreatePendulum(-145, 131);
	CreatePendulum(-166, 72);
	CreatePendulum(-152, 28);
	CreatePendulum(-150, 16);
	CreatePendulum(-145, 0);
	CreatePendulum(-149, -129);
	CreatePendulum(-148, -218);
	CreatePendulum(-84, -286);
	CreatePendulum(-45, -283);
	CreatePendulum(57, -335);
	CreatePendulum(62, -375);
	CreatePendulum(58, -436);
	CreatePendulum(114, -458);
	CreatePendulum(113, -409);
	CreatePendulum(116,-374);
	CreatePendulum(114, -328);
	CreatePendulum(73, -230);
	CreatePendulum(207, -121);
	CreatePendulum(172, -123);
	CreatePendulum(131, -127);
	CreatePendulum(0, -188);
	CreatePendulum(-31, -208);
	CreatePendulum(-26, -56);
	CreatePendulum(-2, 8);
	
	// Nitro Objects
	NitroObject({ -23.78F, 1, 95.70F}, 3, 20); 
	NitroObject({ -129.81F, 1, 124.58F}, 1, 20);
	NitroObject({ -129.81F, 1, 128.58F}, 1, 20);
	NitroObject({ -129.81F, 1, 132.58F}, 1, 20);//checkpoint
	NitroObject({ -171.57F, 1, -17.52F }, 3, 20);
	NitroObject({ -170.91F, 1, -195.71F}, 3, 20);
	NitroObject({ -29.24F, 1, -281.81F}, 1, 20);//checkpoint
	NitroObject({ -29.24F, 1, -285.81F}, 1, 20);
	NitroObject({ -29.24F, 1, -289.81F}, 1, 20);
	NitroObject({ 35.08F, 1, -405.92F}, 3, 20);
	NitroObject({ 92.05F, 1, -435.15F}, 3, 20);
	NitroObject({ 33.71F, 1, -204.62F }, 3, 20);//checkpoint
	NitroObject({ 220.48F, 1, -121.50F}, 1, 20);
	NitroObject({ 220.48F, 1, -125.50F}, 1, 20);
	NitroObject({ 220.48F, 1, -129.50F}, 1, 20);
	NitroObject({ 98.90F, 1, -120.14F}, 1, 20);
	NitroObject({ 98.90F, 1, -124.14F}, 1, 20);
	NitroObject({ 98.90F, 1, -128.14F}, 1, 20);
	NitroObject({ -21.62F, 1, -167.14F }, 3, 20);
	NitroObject({ -77.66F, 1, -103.31F}, 3, 20);

	//Checkpoint Objects
	CreateCheckpoint({ -129.81F, 1, 128.58F }, false);
	CreateCheckpoint({ -29.24F, 1, -284.81F }, false);
	CreateCheckpoint({ 60.71F, 1, -200.62F }, true);
	CreateCheckpoint({ 0, 0.44F, 40.71F }, true, PhysBody3D::Tag::CHECKPOINT_FINISH);

	firstLap = true;

	// Timer
	timer.Start();

	game_over = false;
	start_game = false;

	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	current_time = SDL_GetTicks() - start_time;

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		camera_free = !camera_free;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (game_over || game_win)) {
		ResetVars();
	}

	//Render Map 
	for (int i = 0; i < map.Count(); i++)
	{
		map[i]->Render();
	}

	for (int i = 0; i < pendulumBall_body.Count(); i++) {
		pendulumBall_body[i]->GetTransform(&pendulumBall_shape[i]->transform);
		pendulumBall_shape[i]->Render();
	}
	
	// Nitro Scene Objects
	for (uint i = 0; i < nitro_objects.Count(); i++)
	{
		if(nitro_objects[i]->active)
			nitro_objects[i]->Render();
	}

	// Checkpoints Scene Objects
	for (uint i = 0; i < checkpoint_objects.Count(); i++)
	{
		if (checkpoint_objects[i]->active)
			checkpoint_objects[i]->Render();

		if (map[i]->active)
			map[i]->Render();
	}

	if(timer.Read()/1000 == 180)
		App->audio->PlayMusic("Audio/Music/60 Seconds.ogg");

	char title[280];
	if (!game_over & !game_win)
	{
		sprintf_s(title, "Velocity: %.1F Km/h || Nitro: %d || Checkpoints: %i || Lap: %i/3 Time: %.2d:%.2d || Fastest Lap: %i Time: %.2d:%.2d                                                                                      Timer: %.2d:%.2d",
			App->player->vehicle->GetKmh(), App->player->nitro, checkpoints, laps, timer_lap.Read() / 60000, timer_lap.Read() / 1000 % 60, fastest_lap, fastest_lap_time / 60000, fastest_lap_time / 1000 % 60, timer.Read() / 60000, timer.Read() / 1000 % 60);
	}
	else if(game_over)
	{
		sprintf_s(title, "Timer: %.2d:%.2d || Lap: %i/3 Time: %.2d:%.2d || Fastest Lap: %i Time: %.2d:%.2d ||           GAME OVER!!!!!!!!!!              Reset Game by Pressing ENTER", timer.Read() / 60000, timer.Read() / 1000 % 60, laps,
			timer_lap.Read() / 60000, timer_lap.Read() / 1000 % 60, fastest_lap, fastest_lap_time / 60000, fastest_lap_time / 1000 % 60);
	}
	else if (game_win)
	{
		sprintf_s(title, "Timer: %.2d:%.2d || Lap: %i/3 Time: %.2d:%.2d || Fastest Lap: %i Time: %.2d:%.2d ||          WINNER WINNER CHICKEN DINNER!!!!!!!!!!           Thank you so much for playing our Game!", timer.Read() / 60000, timer.Read() / 1000 % 60, laps,
			timer_lap.Read() / 60000, timer_lap.Read() / 1000 % 60, fastest_lap, fastest_lap_time / 60000, fastest_lap_time / 1000 % 60);
	}
	

	// Lose condition
	if (timer.Read()/1000 >= 240 && start_game)
	{
		App->player->SaveGhostData(false);
		LOG("*******GAME OVER**********");
		camera_free = true;

		App->camera->Position.Set(0, 50, 0);
		App->camera->LookAt(App->player->vehicle->GetPosition());
		game_over = true;
	}

	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (uint i = 0; i < map.Count(); ++i)
		delete map[i];

	for (int i = 0; i < nitro_objects.Count(); i++)
		delete nitro_objects[i];

	for (int i = 0; i < checkpoint_objects.Count(); i++)
		delete checkpoint_objects[i];

	for (int i = 0; i < pendulumBall_shape.Count(); i++)
		delete pendulumBall_shape[i];

	return true;
}

void ModuleSceneIntro::CreateRect(const float & x, const float & y, const float & z, const float & width, const float &length, const Cube & cube, ORIENTATION orientation)
{
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	PhysBody3D* phys1 = nullptr;
	PhysBody3D* phys2 = nullptr;
	
	for (int i = 0; i < length; i=i+3) {
		c1 = new Cube(cube);
		c2 = new Cube(cube);
		
		switch (orientation)
		{
		case ORIENTATION::NORTH:
			c1->SetPos(x, y, z + cube.size.x*i);
			c2->SetPos(x + width, y, z + cube.size.x*i);
			break;
		case ORIENTATION::SOUTH:
			c1->SetPos(x, y, z - cube.size.x*i);
			c2->SetPos(x + width, y, z - cube.size.x*i);
			break;
		case ORIENTATION::EAST:
			c1->SetPos(x - cube.size.x*i, y, z);
			c2->SetPos(x - cube.size.x*i, y, z + width);
			break;
		case ORIENTATION::WEST:
			c1->SetPos(x + cube.size.x*i, y, z);
			c2->SetPos(x + cube.size.x*i, y, z + width);
			break;
		case ORIENTATION::NORTHEAST:
			c1->SetPos(x - cube.size.x*i, y, z + cube.size.x*i);
			c2->SetPos(x + width - cube.size.x*i, y, z + cube.size.x*i);
			break;
		case ORIENTATION::NORTHWEST:
			c1->SetPos(x + cube.size.x*i, y, z + cube.size.x*i);
			c2->SetPos(x + width + cube.size.x*i, y, z + cube.size.x*i);
			break;
		case ORIENTATION::SOUTHEAST:
			c1->SetPos(x - cube.size.x*i, y, z - cube.size.x*i);
			c2->SetPos(x - cube.size.x*i + width, y, z - cube.size.x*i);
			break;
		case ORIENTATION::SOUTHWEST:
			c1->SetPos(x + cube.size.x*i, y, z - cube.size.x*i);
			c2->SetPos(x + cube.size.x*i + width, y, z - cube.size.x*i);
			break;
		default:
			break;
		}
		phys1 = App->physics->AddBody(*c1, 0.0F);
		phys2 = App->physics->AddBody(*c2, 0.0F);
	
		phys1->SetState(PhysBody3D::Tag::WALL);
		phys2->SetState(PhysBody3D::Tag::WALL);

		map.PushBack(c1);
		map.PushBack(c2);
	}	
}

void ModuleSceneIntro::CreateCurve(const float & x, const float & y, const float & z, const float & width, const float & angle_i, const float & angle_f, const Cube & cube, const float & radius)
{
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	PhysBody3D* curve1 = nullptr;
	PhysBody3D* curve2 = nullptr;
	
	for (int i = angle_i; i <= angle_f; i = i + cube.size.x * 5) {
		c1 = new Cube(cube);
		c2 = new Cube(cube);
		
		c1->SetPos(x + radius * cos(i*3.1415 / 180), y, z + radius * sin(i*3.1415 / 180));
		c2->SetPos(x + (radius + width) * cos(i*3.1415 / 180), y, z + (radius + width) * sin(i*3.1415 / 180));
		
		curve1 = App->physics->AddBody(*c1, 0.0F);
		curve2 = App->physics->AddBody(*c2, 0.0F);

		curve1->SetState(PhysBody3D::Tag::WALL);
		curve2->SetState(PhysBody3D::Tag::WALL);

		map.PushBack(c1);
		map.PushBack(c2);
	}
	
}

void ModuleSceneIntro::CreatePendulum(const float & x, const float & z)
{
	int support_high = 15;
	Cube* support_shape = new Cube();
	Sphere* ball_shape = new Sphere(2);

	support_shape->SetPos(x, support_high, z);
	support_shape->color.Set(0, 0, 0);
	ball_shape->color.Set(1, 2, 3);
	ball_shape->SetPos(x, 0.2F, z);

	PhysBody3D* support = App->physics->AddBody(*support_shape, 0.0F);
	PhysBody3D* ball = App->physics->AddBody(*ball_shape, 200.0F);

	map.PushBack(support_shape);
	pendulumBall_shape.PushBack(ball_shape);
	pendulumBall_body.PushBack(ball);

	App->physics->AddConstraintP2P(*support, *ball, vec3(0,0,0), vec3(0,12,0));
}

void ModuleSceneIntro::NitroObject(vec3 pos, int size, int distance_to)
{
	Sphere* nitro_obj = nullptr;
	for (int i = 0; i < size; i++)
	{
		nitro_obj = new Sphere(1);
		nitro_obj->color.Set(3, 2, 1);
		nitro_obj->SetPos(pos.x + distance_to, pos.y, pos.z);
		nitro_obj->wire = true;
		nitro_objects.PushBack(nitro_obj);
		distance_to += 4;
		PhysBody3D* sensor = App->physics->AddBody(*nitro_obj, 0);
		sensor->SetAsSensor(true);
		sensor->SetState(PhysBody3D::Tag::NITRO);
		nitro_objects_body.PushBack(sensor);
	}
}

void ModuleSceneIntro::PickUpNitroObject(PhysBody3D * nitro_body)
{
	for (int i = 0; i < nitro_objects_body.Count(); i++)
	{
		if (nitro_objects_body[i] == nitro_body)
		{
			nitro_objects[i]->active = false;
			break;
		}
	}
	
}

void ModuleSceneIntro::CreateCheckpoint(vec3 pos, bool rotate, PhysBody3D::Tag type)
{
	Cube* checkpoint_obj = nullptr;
	Cube* green_cube = nullptr;
	if (rotate)
	{
		checkpoint_obj = new Cube(15, 1, 1);
		green_cube = new Cube(15, 1, 1);
	}
	else
	{
		checkpoint_obj = new Cube(1, 1, 15);
		green_cube = new Cube(1, 1, 15);

	}


	checkpoint_obj->SetPos(pos.x, pos.y+1, pos.z);
	checkpoint_objects.PushBack(checkpoint_obj);
	PhysBody3D* sensor = App->physics->AddBody(*checkpoint_obj, 0);
	sensor->SetAsSensor(true);
	sensor->SetState(type);
	checkpoint_objects_body.PushBack(sensor);

	if (type == PhysBody3D::Tag::CHECKPOINT)
		green_cube->color.Set(0, 255, 0);
	else
		green_cube->color.Set(0, 255, 255);

	green_cube->SetPos(pos.x, pos.y+ 4, pos.z);
	map.PushBack(green_cube);

}

void ModuleSceneIntro::Checkpoint(PhysBody3D* checkpoint_body)
{
	for (uint i = 0; i < checkpoint_objects_body.Count(); i++)
	{
		if (checkpoint_objects_body[i] == checkpoint_body)
		{
			checkpoint_objects[i]->active = false;
			break;
		}
	}
}

void ModuleSceneIntro::ResetVars()
{
	for (int i = 0; i < nitro_objects_body.Count(); i++) {
		nitro_objects_body[i]->SetActive(true);
		nitro_objects[i]->active = true;
	}

	for (int i = 0; i < checkpoint_objects_body.Count(); i++) {
		checkpoint_objects_body[i]->SetActive(true);
		checkpoint_objects[i]->active = true;
	}

	for (int i = 0; i < pendulumBall_body.Count(); i++) {
		pendulumBall_body[i]->SetActive(true);
		pendulumBall_shape[i]->active = true;
	}
	
	firstLap = true;
	laps = 0;
	timer_lap.Start();
	timer.Start();
	checkpoints = 0;
	App->player->Reset();
	game_over = false;
	start_game = false;
	camera_free = false;
	game_win = false;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	switch (body2->GetState())
	{
	case PhysBody3D::Tag::NITRO:
		PickUpNitroObject(body2);
		body2->SetActive(false);
		App->player->nitro = true;
		App->player->vehicle->nitro_off = 0;
		App->player->vehicle->nitro_on = 255;

		if (current_time >= 1000)
		{
			App->audio->PlayFx(App->player->fx_nitro_pick_up);
			start_time = SDL_GetTicks();
		}
		break;

	case PhysBody3D::Tag::CHECKPOINT:
		Checkpoint(body2);
		body2->SetActive(false);
		App->player->SetCheckpointPosition();
		checkpoints++;
		if(checkpoints>=3)
			for(int i =0;i<checkpoint_objects_body.Count();i++)
				if (checkpoint_objects_body[i]->GetState() == PhysBody3D::Tag::CHECKPOINT_FINISH) {
					checkpoint_objects_body[i]->SetActive(true);
					checkpoint_objects[i]->active = true;
					firstLap = false;
				}
		App->audio->PlayFx(App->player->fx_checkpoint);
		break;

	case PhysBody3D::Tag::CHECKPOINT_FINISH:
		if (firstLap) {
			timer.Start();
			timer_lap.Start();
			fastest_lap_time = 400000;
			App->player->SaveGhostData();
		}
		else {
			App->player->IterateGhost();
			if (timer_lap.Read() < fastest_lap_time) {
				fastest_lap_time = timer_lap.Read();
				fastest_lap = laps;
			}
		}

		checkpoints = 0;
		for (int i = 0; i < checkpoint_objects_body.Count(); i++) {
			if (checkpoint_objects[i] != nullptr)
				checkpoint_objects[i]->active = true;
			if (checkpoint_objects_body[i] != nullptr)
				checkpoint_objects_body[i]->SetActive(true);
		}
		Checkpoint(body2);
		body2->SetActive(false);
		App->player->SetCheckpointPosition();

		App->audio->PlayFx(App->player->fx_checkpoint);
		timer_lap.Start();

		// Win Condition
		if (laps >= 3 && timer.Read()/1000 <= 240)
		{
			App->player->SaveGhostData(false);
			LOG("*******WINNER WINNER CHICKER DINNER**********");
			camera_free = true;

			App->camera->Position.Set(0, 300, 0);
			App->camera->LookAt(App->player->vehicle->GetPosition());
			game_over = false;
			game_win = true;
		}
		else
			laps++;

		start_game = true;
		break;

	case PhysBody3D::Tag::WALL:
		if (current_time >= 500 && !game_over)
		{
			App->audio->PlayFx(App->player->fx_crash);
			start_time = SDL_GetTicks();
		}
		break;
	default:
		break;
	}
}

//void ModuleSceneIntro::CreateRamp(const float & x, const float & y, const float & z, const float & inclination, const float & rotation)
//{
//	Cube* c = new Cube(10, 10, 10);
//	c->SetPos(x, y, z);
//	mat4x4 mat = c->transform;
//	vec3 u(0,0,0);
//	float angle = 0.0f;
//	Angles2AxisAngle(inclination, rotation,u,angle);
//	LOG("axis %.2f %.2f %.2f angle %.2f", u.x, u.y, u.z, angle);
//	c->SetRotation(angle, u);
//
//	//c->SetRotation(rotation, vec3(0.0f, 1.0f, 0.0f));
//	PhysBody3D* phys = App->physics->AddBody(*c, 0.0f);
//	
//	map.PushBack(c);
//}

//void ModuleSceneIntro::Angles2AxisAngle(const float & alpha_x, const float & beta_y, vec3 & u, float & angle)
//{
//	double c1 = cos(alpha_x / 2);
//	double s1 = sin(alpha_x / 2);
//	double c2 = cos(beta_y / 2);
//	double s2 = sin(beta_y / 2);
//	double c3 = cos(0 / 2);
//	double s3 = cos(0 / 2);
//	double c1c2 = c1 * c2;
//	double s1s2 = s1 * s2;
//	int w = c1c2 * c3 - s1s2 * s3;
//	int x = c1c2 * s3 + s1s2 * c3;
//	int y = s1 * c2*c3 + c1 * s2*s3;
//	int z = c1 * s2*c3 - s1 * c2*s3;
//	angle = 2 * acos(w);
//	double norm = x * x + y * y + z * z;
//	if (norm < 0.001) { // when all euler angles are zero angle = 0 so
//		// we can set axis to anything to avoid divide by zero
//		x = 1;
//		y = z = 0;
//	}
//	else {
//		norm = sqrt(norm);
//		x /= norm;
//		y /= norm;
//		z /= norm;
//	}
//	u = vec3(x, y, z);
//}