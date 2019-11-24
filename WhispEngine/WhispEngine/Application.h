#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "FileSystem.h"
#include "Random.h"
#include "Random.h"
#include "JsonHelper.h"
#include "HardwareInfo.h"

#include <list>

class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleGUI;
class ModuleShortCut;
class ModuleObjectManager;
class ModuleImport;
class ModuleResources;

class Application
{
public:
	ModuleWindow*			window			= nullptr;
	ModuleInput*			input			= nullptr;
	ModuleSceneIntro*		scene_intro		= nullptr;
	ModuleRenderer3D*		renderer3D		= nullptr;
	ModuleCamera3D*			camera			= nullptr;
	ModuleGUI*				gui				= nullptr;
	ModuleShortCut*			shortcut		= nullptr;
	ModuleObjectManager*	object_manager	= nullptr;
	ModuleImport*			importer		= nullptr;
	ModuleResources*		resources		= nullptr;
	//ModuleFileSystem* file_system = nullptr;

public:

	uint	frame_count = 0;
	int		framerate_cap = 60;
	bool	apply_cap_frames = true;
	bool	pause_game = false;

	uint	last_sec_frame_count = 0;
	uint	prev_last_sec_frame_count = 0;
	double last_frame_ms = 0;


private:
	std::string engine_name;
	std::string organization;
	std::string version;

	float	dt = 0.0f;

	Timer		last_sec_frame_time;
	PerfTimer   perfect_frame_time;
	Timer		frame_time;
	Timer		start_time;

	std::list<Module*> list_modules;

	bool want_to_save = false;
	bool want_to_load = false;
	bool want_to_load_def = false;

	bool want_to_save_scene = false;
	bool want_to_load_scene = false;
	std::string scene_path;

	std::string log_buff;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetDeltaTime();
	const char* GetLog();
	void Log(const char*);
	void SaveLogFile();
	void SaveConfiguration();
	void LoadConfiguration();
	void SaveScene();
	void LoadScene(const char * path);
	void LoadDefaultConfiguration();

	void SetAppName(const char* name);
	void SetOrganizationName(const char* name);
	const char* GetAppName() const;
	const char* GetOrganizationName() const;
	const char* GetVersion() const;

	// Helpers
	HardwareInfo *hardware = nullptr;
	FileSystem *dummy_file_system = nullptr;
	Random* random = nullptr;
	JsonHelper* json = nullptr;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool SaveConfNow();
	bool LoadConfNow();
};

extern Application* App;