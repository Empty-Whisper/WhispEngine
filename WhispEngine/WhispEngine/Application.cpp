#include "Application.h"

#include <fstream>

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);

	AddModule(gui);

	// Renderer last!
	AddModule(renderer3D);

	hardware = new HardwareInfo();
	file_system = new FileSystem();

	first_frame = true;
}

Application::~Application()
{
	for (auto item = list_modules.rbegin(); item != list_modules.rend(); item++) {
		delete *item;
		*item = nullptr;
	}

	list_modules.clear();

}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (auto item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (auto item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Start();
	}
	
	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	if (pause_game)
	{
		dt = 0.0f;
	}
	else if (first_frame)
	{
		
		dt = 1.0f / framerate_cap;
	}
	else
	{
		double framerate = 1000.00 / perfect_frame_time.ReadMs();

		dt = 1.0f / framerate;

		if (dt > 1.0f / (float)framerate_cap + 0.02f)
		{
			dt = 1.0f / (float)framerate_cap + 0.02f;
		}
	}

	perfect_frame_time.Start();

	// Change frame cap/Pause Game ========================================
	if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		apply_cap_frames = !apply_cap_frames;
	}
	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		pause_game = !pause_game;
	}
}


float Application::GetDeltaTime()
{
	return dt;
}

const char * Application::GetLog()
{
	return log_buff.data();
}

void Application::Log(const char * str)
{
	log_buff.append(str); //Save log data to export in txt when Engine close

	gui->Log(str); //Send to Console log engine
}

void Application::SaveLogFile()
{
	std::ofstream log_output("Log.log");
	log_output << log_buff;
	log_output.close();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (auto item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PreUpdate();
	}

	for (auto item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->Update();
	}

	for (auto item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PostUpdate();
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{

	if (first_frame == true)
	{
		first_frame = false;
	}

	if (last_sec_frame_time.Read() >= 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	last_frame_ms = perfect_frame_time.ReadMs();

	if (!VSYNC && framerate_cap != 0 && apply_cap_frames)
	{
		
		Uint32 frame_cap_ms = 1000.0F / (float)framerate_cap;

		if (frame_cap_ms > last_frame_ms)
		{
			SDL_Delay((Uint32)frame_cap_ms - last_frame_ms);
		}
		else
			SDL_Delay(frame_cap_ms - (Uint32)last_frame_ms % (Uint32)frame_cap_ms);
	}
}

bool Application::CleanUp()
{
	bool ret = true;
	
	for (auto item = list_modules.rbegin(); item != list_modules.rend() && ret; item++) {
		ret = (*item)->CleanUp();
	}

	delete hardware;
	hardware = nullptr;
	delete file_system;
	file_system = nullptr;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}