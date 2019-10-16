#include "Application.h"

#include <fstream>

Application::Application()
{
	window =			new ModuleWindow();
	input =				new ModuleInput();
	scene_intro =		new ModuleSceneIntro();
	renderer3D =		new ModuleRenderer3D();
	camera =			new ModuleCamera3D();
	gui =				new ModuleGUI();
	shortcut =			new ModuleShortCut();
	importer =			new ModuleImport();
	object_manager =	new ModuleObjectManager();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(shortcut);
	AddModule(importer);
	AddModule(object_manager);
	
	// Scenes
	AddModule(scene_intro);


	// Renderer last!
	AddModule(gui);
	AddModule(renderer3D);

	hardware = new HardwareInfo();
	file_system = new FileSystem();
	random = new Random();

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

	nlohmann::json load = file_system->OpenFile("Configuration/configuration.json");
	if(load == nullptr) load = file_system->OpenFile("Configuration/conf_default.json");

	engine_name = load["Configuration"]["App"]["name"].get<std::string>();
	organization = load["Configuration"]["App"]["organization"].get<std::string>();
	//version = load["Configuration"]["App"]["version"].get<std::string>();

	// Call Init() in all modules
	for (auto item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Init(load["Configuration"][(*item)->name.data()]);
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

void Application::SaveConfiguration()
{
	want_to_save = true;
}

void Application::LoadConfiguration()
{
	want_to_load = true;
}

void Application::LoadDefaultConfiguration()
{
	want_to_load_def = true;
}

void Application::SetAppName(const char * name)
{
	engine_name = name;
}

void Application::SetOrganizationName(const char * name)
{
	organization = name;
}

const char * Application::GetAppName() const
{
	return engine_name.data();
}

const char * Application::GetOrganizationName() const
{
	return organization.data();
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
	if (want_to_save) {
		SaveConfNow();
	}

	if (want_to_load || want_to_load_def) {
		LoadConfNow();
	}

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

	if (framerate_cap != 0 && apply_cap_frames)
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

bool Application::SaveConfNow()
{
	bool ret = true;

	nlohmann::json save = file_system->OpenFile("configuration.json");
	
	save["Configuration"]["App"]["name"] = engine_name.data();
	save["Configuration"]["App"]["organization"] = organization.data();
	save["Configuration"]["App"]["version"] = 0;
	
	for (auto i = list_modules.begin(); i != list_modules.end(); ++i) {
		(*i)->Save(save["Configuration"][(*i)->name.data()]);
	}

	file_system->SaveFile("configuration.json", save);

	want_to_save = false;

	return ret;
}

bool Application::LoadConfNow()
{
	bool ret = true;

	nlohmann::json load;

	if (want_to_load)
		load = file_system->OpenFile("configuration.json");
	else if(want_to_load_def)
		load = file_system->OpenFile("conf_default.json");

	engine_name = load["Configuration"]["App"]["name"].get<std::string>();
	organization = load["Configuration"]["App"]["organization"].get<std::string>();
	//version = load["Configuration"]["App"]["version"].get<std::string>();

	for (auto i = list_modules.begin(); i != list_modules.end(); ++i) {
		(*i)->Load(load["Configuration"][(*i)->name.data()]);
	}

	want_to_load = false;
	want_to_load_def = false;

	return ret;
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

	delete random;
	random = nullptr;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}