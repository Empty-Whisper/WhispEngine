#include "Application.h"
#include "Brofiler/Brofiler.h"
#include <fstream>

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"
#include "ModuleShortCut.h"
#include "ModuleImport.h"
#include "ModuleObjectManager.h"
#include "ModuleResources.h"
#include "ModuleScripting.h"

#include "Lua/LuaBridge/LuaBridge.h"

#include "Time.h"

GameTime* Time = nullptr;

Application::Application()
{
	Time = new GameTime();

	window =			new ModuleWindow();
	input =				new ModuleInput();
	scene_intro =		new ModuleSceneIntro();
	renderer3D =		new ModuleRenderer3D();
	camera =			new ModuleCamera3D();
	gui =				new ModuleGUI();
	shortcut =			new ModuleShortCut();
	importer =			new ModuleImport();
	object_manager =	new ModuleObjectManager();
	resources =			new ModuleResources();
	scripting =			new ModuleScripting();
	//file_system_fs =		new ModuleFileSystem();		------------Unused for now, in opposite we use our own FileSystem--------------------

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(resources);
	//AddModule(file_system_fs);
	AddModule(shortcut);
	AddModule(importer);
	AddModule(object_manager);
	AddModule(scripting);
	
	// Scenes
	AddModule(scene_intro);


	// Renderer last!
	AddModule(gui);
	AddModule(renderer3D);

	hardware = new HardwareInfo();
	file_system = new FileSystem();
	random = new Random();
	json = new JsonHelper();
}

Application::~Application()
{
	for (auto item = list_modules.rbegin(); item != list_modules.rend(); item++) {
		delete *item;
		*item = nullptr;
	}

	list_modules.clear();

	delete Time;
}

bool Application::Init()
{
	bool ret = true;

	nlohmann::json load = file_system->OpenFile("Configuration/configuration.json");
	if(load == nullptr) load = file_system->OpenFile("Configuration/conf_default.json");

	engine_name = load["Configuration"]["App"]["name"].get<std::string>();
	organization = load["Configuration"]["App"]["organization"].get<std::string>();
	version = load["Configuration"]["App"]["version"].get<std::string>();

	// Call Init() in all modules
	for (auto item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Init(load["Configuration"][(*item)->name.data()]);
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (auto item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Start();
	}
	
	App->LuaRegister();
	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	BROFILER_CATEGORY("PrepareUpdate", Profiler::Color::Blue);
	frame_count++;
	last_sec_frame_count++;
	dt = frame_time.ReadSec();
	frame_time.Start();

	switch (state)
	{
	case Application::NONE:
		break;
	case Application::PLAY:
		Time->Start();
		Time->SetDeltaTime(dt);
		state = GameState::PLAYING;
		break;
	case Application::PLAYING:
		Time->SetDeltaTime(dt);
		break;
	case Application::REANUDE:
		Time->Reanude();
		Time->SetDeltaTime(dt);
		state = GameState::PLAYING;
		break;
	case Application::PAUSE:
		Time->SetDeltaTime(0.f);
		Time->Pause();
		state = GameState::PAUSED;
		break;
	case Application::PAUSED:
		Time->SetDeltaTime(0.f);
		break;
	case Application::STOP:
		Time->Stop();
		Time->SetDeltaTime(0.f);
		state = GameState::NONE;
		break;
	case Application::ONE_FRAME:
		Time->Reanude();
		Time->SetDeltaTime(dt);
		state = GameState::PAUSE;
		break;
	default:
		break;
	}

	perfect_frame_time.Start();
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

void Application::SaveScene()
{
	want_to_save_scene = true;
}

void Application::LoadScene(const char * path)
{
	want_to_load_scene = true;
	scene_path.assign(path);
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

const char * Application::GetVersion() const
{
	return version.data();
}

void Application::LuaRegister()
{
	LOG("Registering all lua functions to enable scripting");
	for (auto i = list_modules.begin(); i != list_modules.end(); i++) {
		(*i)->LuaRegister();
	}

	luabridge::getGlobalNamespace(scripting->GetState())
		.beginNamespace("Time")
			.addProperty("deltaTime", &Time->dt, false)
		.endNamespace()
		.beginNamespace("Random")
			.addFunction("Rangef", &Random::Randomf)
			.addFunction("Rangei", &Random::Randomi)
		.endNamespace();
}

void Application::SetState(const GameState to_state)
{
#ifdef _DEBUG //We only want to get that message error while developing
	std::string log("Cannot send ");
	bool fail = false;
	switch (to_state)
	{
	case Application::NONE:
		log.append("NONE");
		fail = true;
		break;
	case Application::PLAYING:
		log.append("PLAYING");
		fail = true;
		break;
	case Application::PAUSED:
		log.append("PAUSED");
		fail = true;
		break;
	case Application::MAX:
		log.append("MAX");
		fail = true;
		break;
	}
	if (fail) {
		log.append(" to SetState(), enums availables are PLAY, PAUSE, STOP and ONE_FRAME");
		return;
	}
#endif // DEBUG

	state = to_state;
}

Application::GameState Application::GetState() const
{
	return state;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	BROFILER_CATEGORY("Application", Profiler::Color::Blue);
	update_status ret = UPDATE_CONTINUE;

	PrepareUpdate();
	PreUpdate(ret);
	DoUpdate(ret);
	PostUpdate(ret);
	FinishUpdate();

	return ret;
}

void Application::PostUpdate(update_status &ret)
{
	BROFILER_CATEGORY("PostUpdate", Profiler::Color::Blue);
	for (auto item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PostUpdate();
		if (ret != UPDATE_CONTINUE)
			LOG("%s returned UPDATE %i", (*item)->name.data(), ret);
	}
}

void Application::DoUpdate(update_status &ret)
{
	BROFILER_CATEGORY("Update", Profiler::Color::Blue);
	for (auto item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->Update();
		if (ret != UPDATE_CONTINUE)
			LOG("%s returned UPDATE %i", (*item)->name.data(), ret);
	}
}

void Application::PreUpdate(update_status &ret)
{
	BROFILER_CATEGORY("PreUpdate", Profiler::Color::Blue);
	for (auto item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PreUpdate();
		if (ret != UPDATE_CONTINUE)
			LOG("%s returned UPDATE %i", (*item)->name.data(), ret);
	}
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	BROFILER_CATEGORY("FinishUpdate", Profiler::Color::Blue);
	if (want_to_save) {
		SaveConfNow();
	}

	if (want_to_save_scene) {
		want_to_save_scene = false;
		scene_intro->SaveCurrentScene();
	}

	if (want_to_load_scene) {
		want_to_load_scene = false;
		scene_intro->LoadScene(scene_path.c_str());
		scene_path.clear();
	}

	if (want_to_load || want_to_load_def) {
		LoadConfNow();
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
		
		Uint32 frame_cap_ms = 1000.0F / (Uint32)framerate_cap;

		if (frame_cap_ms > last_frame_ms)
		{
			SDL_Delay(frame_cap_ms - (Uint32)last_frame_ms);
		}
		else
			SDL_Delay(frame_cap_ms - (Uint32)last_frame_ms % (Uint32)frame_cap_ms);
	}
}

bool Application::SaveConfNow()
{
	bool ret = true;

	nlohmann::json save = file_system->OpenFile("Configuration/configuration.json");
	
	save["Configuration"]["App"]["name"] = engine_name.data();
	save["Configuration"]["App"]["organization"] = organization.data();
	save["Configuration"]["App"]["version"] = version.data();
	
	for (auto i = list_modules.begin(); i != list_modules.end(); ++i) {
		(*i)->Save(save["Configuration"][(*i)->name.data()]);
	}

	file_system->SaveFile("Configuration/configuration.json", save);

	want_to_save = false;

	return ret;
}

bool Application::LoadConfNow()
{
	bool ret = true;

	nlohmann::json load;

	if (want_to_load)
		load = file_system->OpenFile("Configuration/configuration.json");
	else if(want_to_load_def)
		load = file_system->OpenFile("Configuration/conf_default.json");

	engine_name = load["Configuration"]["App"]["name"].get<std::string>();
	organization = load["Configuration"]["App"]["organization"].get<std::string>();
	version = load["Configuration"]["App"]["version"].get<std::string>();

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

	delete json;
	json = nullptr;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}