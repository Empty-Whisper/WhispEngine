#include <fstream>
#include <iomanip>

#include "Application.h"
#include "Globals.h"
#include "ModuleGUI.h"

#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelAbout.h"
#include "PanelObjects.h"
#include "PanelCreate.h"


ModuleGUI::ModuleGUI(bool enable_true) :Module(enable_true)
{
	name.assign("UI");
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init(nlohmann::json &node)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_VERSION));
	ImGui::StyleColorsDark();

	panels.push_back(config = new PanelConfiguration(node["panels"]["configuration"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_2));
	panels.push_back(about = new PanelAbout(node["panels"]["about"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_LCTRL, SDL_SCANCODE_A));
	panels.push_back(console = new PanelConsole(node["panels"]["console"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_1));
	panels.push_back(inspector = new PanelObjects());
	panels.push_back(create = new PanelCreate());
	//panels.push_back(create = new PanelCreate(node["panels"]["create"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_4));

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	
	ImGui::NewFrame();

	//  Input Shortcut Keys
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKeyDown(SDL_SCANCODE_3))
	{
		show_style_window = !show_style_window;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	update_status ret = MainMenuBar();

	for (auto i = panels.begin(); i != panels.end(); ++i) {
		if ((*i)->IsActive()) {
			(*i)->Update();
		}
	}

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (show_style_window)
	{
		if (ImGui::Begin("Style editor"))
		{
			ImGui::ShowStyleEditor();
		}
		ImGui::End();
	}

	return ret;
}

update_status ModuleGUI::MainMenuBar()
{
	update_status ret = UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				ret = update_status::UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Console", "LShift+1", &console->active);
			ImGui::MenuItem("Configuration", "LShift+2", &config->active);
			ImGui::MenuItem("Style Editor", "LShift+3", &show_style_window);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Shape"))
		{
			ImGui::MenuItem("Create", "LShift+4", &create->active);

			if (ImGui::MenuItem("Demo", NULL, &show_demo_create))
			{
				App->object_manager->Demo();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Demo", NULL, show_demo_window))
			{
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("Documentation", "link"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Empty-Whisper/WhispEngine", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download Lastest", "link"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Empty-Whisper/WhispEngine/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug", "link"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Empty-Whisper/WhispEngine/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::MenuItem("About", "LControl+LShift+A", &about->active);

			ImGui::EndMenu();
		}		
	}
	ImGui::EndMainMenuBar();

	return ret;
}

update_status ModuleGUI::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {
	for (auto i = panels.begin(); i != panels.end(); ++i) {
		delete *i;
	}
	panels.clear();
	console = nullptr;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleGUI::Log(const char * str)
{
	if (console != nullptr) {
		console->Log(str);
	}
}

