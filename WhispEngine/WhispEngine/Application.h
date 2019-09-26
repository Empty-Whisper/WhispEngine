#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"

#include "HardwareInfo.h"

#include <list>

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleGUI* gui = nullptr;

public:

	uint	frame_count = 0;
	int		framerate_cap = 60;
	bool	apply_cap_frames = true;
	bool	pause_game = false;

	uint	last_sec_frame_count = 0;
	uint	prev_last_sec_frame_count = 0;
	double last_frame_ms = 0;


private:

	bool    first_frame = true;
	float	dt = 0.0f;

	Timer		last_sec_frame_time;
	PerfTimer   perfect_frame_time;
	Timer		frame_time;
	Timer		start_time;

	std::list<Module*> list_modules;

	std::string log_buff;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetDeltaTime();
	void Log(const char*);
	void SaveLogFile();

	HardwareInfo *hardware = nullptr;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;