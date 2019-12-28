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
#include "PanelHierarchy.h"
#include "PanelCreate.h"
#include "PanelInspector.h"
#include "PanelScene.h"
#include "PanelGame.h"
#include "PanelResources.h"
#include "PanelScriptEditor.h"
#include "PanelShortcut.h"

#include "ModuleSceneIntro.h"
#include "ModuleScripting.h"

#include "Time.h"

#include "Brofiler/Brofiler.h"


ModuleGUI::ModuleGUI(bool enable_true) :Module(enable_true)
{
	name.assign("UI");
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init(nlohmann::json &node)
{
	LOG("Initializing module UI...")
	glewInit();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	LOG("Init ImGui v%s", ImGui::GetVersion());
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
#ifndef GAME_BUILD
	// Docking -----------------------------------------------------------
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform 
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
#else
	io.IniFilename = NULL;
#endif
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_VERSION));
	ImGui::StyleColorsDark();

	LOG("Initi panels");
#ifndef GAME_BUILD
	panels.push_back(about = new PanelAbout(node["panels"]["about"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_LCTRL, SDL_SCANCODE_A));
	panels.push_back(config = new PanelConfiguration(node["panels"]["configuration"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_1));
	panels.push_back(console = new PanelConsole(node["panels"]["console"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_2));
	panels.push_back(hierarchy = new PanelHierarchy(node["panels"]["hierarchy"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_3));
	panels.push_back(create = new PanelCreate(node["panels"]["create"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_4));
	panels.push_back(inspector = new PanelInspector(node["panels"]["inspector"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_5));
	panels.push_back(game = new PanelGame(node["panels"]["game"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_7));
	panels.push_back(scene = new PanelScene(node["panels"]["scene"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_6));
	panels.push_back(resources = new PanelResources(node["panels"]["resources"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_8));
	panels.push_back(editor = new PanelScriptEditor(node["panels"]["editor"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_9));
	panels.push_back(shortcut = new PanelShortcut(node["panels"]["shortcut"].value("start_enabled", true), SDL_SCANCODE_LSHIFT, SDL_SCANCODE_0));

	ImGuizmo::Enable(true);
#else
	panels.push_back(game = new PanelGame());
	App->SetState(Application::GameState::PLAY);
#endif

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	BROFILER_CATEGORY("GUI", Profiler::Color::Purple);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	BROFILER_CATEGORY("GUI", Profiler::Color::Purple);
	update_status ret = update_status::UPDATE_CONTINUE;
#ifndef GAME_BUILD
	ret = MainMenuBar();

	ModalSaveScene();

	Dockspace();

	PlayPauseStop();
#endif
	UpdatePanels();
#ifndef GAME_BUILD
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (show_style_window)
	{
		if (ImGui::Begin("Style editor", &show_style_window))
		{
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
	}
#endif
	return ret;
}

void ModuleGUI::UpdatePanels()
{
	BROFILER_CATEGORY("Panels", Profiler::Color::MediumPurple);
	for (auto i = panels.begin(); i != panels.end(); ++i) {
		if ((*i)->IsActive()) {
			(*i)->Update();
		}
	}
}

void ModuleGUI::PlayPauseStop()
{
	BROFILER_CATEGORY("PlayPauseStop", Profiler::Color::DeepPink);
	if (ImGui::Begin("##playandpause", NULL, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		Application::GameState state = App->GetState();
		bool pushed = false;
		if (state == Application::GameState::NONE) {
			ImGui::PushStyleColor(ImGuiCol_Button, IMGUI_COLOR_ORANGE);
			pushed = true;
		}

		if (ImGui::Button("|>")) {
			if (state == Application::GameState::NONE) {
				App->SetState(Application::GameState::PLAY);
				App->scene_intro->SaveTemporaryScene();
			}
			else {
				App->SetState(Application::GameState::STOP);
				App->scene_intro->LoadTemporaryScene();
			}
		}

		ImGui::SameLine();

		if (state != Application::GameState::PAUSED && state != Application::GameState::NONE) {
			ImGui::PushStyleColor(ImGuiCol_Button, IMGUI_COLOR_ORANGE);
			pushed = true;
		}

		if (ImGui::Button("||")) {
			if (state == Application::GameState::PLAYING)
				App->SetState(Application::GameState::PAUSE);
			else if (state == Application::GameState::PAUSED)
				App->SetState(Application::GameState::REANUDE);
		}

		ImGui::SameLine();

		if (state == Application::GameState::PAUSED) {
			ImGui::PushStyleColor(ImGuiCol_Button, IMGUI_COLOR_ORANGE);
			pushed = true;
		}

		if (ImGui::Button("|>|")) {
			if (state == Application::GameState::PAUSED)
				App->SetState(Application::GameState::ONE_FRAME);
		}

		if (pushed)
			ImGui::PopStyleColor();


		ImGui::SameLine();

		ImGui::TextColored(IMGUI_COLOR_ORANGE, "Game Time: ");
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_COLOR_ORANGE, "Seconds Since Play: ");
		ImGui::SameLine();
		ImGui::Text("%.3f", Time->ReadSec());
		ImGui::SameLine();
		ImGui::TextColored(IMGUI_COLOR_ORANGE, "Delta Time: ");
		ImGui::SameLine();
		ImGui::Text("%.3f", Time->GetDeltaTime());

		ImGui::End();
	}
}

void ModuleGUI::ModalSaveScene()
{
	BROFILER_CATEGORY("ModalSaveScene", Profiler::Color::Pink);
	if (open_modal_new_scene)
		ImGui::OpenPopup("Modal Save Scene");
	if (ImGui::BeginPopupModal("Modal Save Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Actual Scene might not be saved since last change. Do you want to save?");

		if (ImGui::Button("Yes, Save and create a new one")) {
			App->SaveScene();
			OpenSaveWindow(true);
			open_modal_new_scene = false;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create without save")) {
			OpenSaveWindow(true);
			open_modal_new_scene = false;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Cancel")) {
			open_modal_new_scene = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

update_status ModuleGUI::MainMenuBar()
{
	BROFILER_CATEGORY("MainMenuBar", Profiler::Color::PeachPuff);
	update_status ret = UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				open_modal_new_scene = true;
			}

			if (ImGui::MenuItem("Save Scene"))
			{
				App->SaveScene();
			}

			if (ImGui::MenuItem("Save Scene as"))
			{
				OpenSaveWindow(false);
			}

			if (ImGui::MenuItem("Load Scene"))
			{
				OpenLoadWindow();
			}

			if (ImGui::MenuItem("Quit"))
			{
				ret = update_status::UPDATE_STOP;
			}
			

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Configuration", "Shift+1", &config->active);
			ImGui::MenuItem("Console", "Shift+2", &console->active);
			ImGui::MenuItem("Hierarchy", "Shift+3", &hierarchy->active);
			ImGui::MenuItem("Create", "Shift+4", &create->active);
			ImGui::MenuItem("Inspector", "Shift+5", &inspector->active);
			ImGui::MenuItem("Scene", "Shift+6", &scene->active);
			ImGui::MenuItem("Game", "Shift+7", &game->active);
			ImGui::MenuItem("Resources", "Shift+8", &resources->active);
			ImGui::MenuItem("Script Editor", "Shift+9", &editor->active);
			ImGui::MenuItem("ShortCut Editor", "Shift+0", &shortcut->active);
			ImGui::MenuItem("Style Editor", "", &show_style_window);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			ImGui::MenuItem("Panel Create", "Shift+4", &create->active);
			ImGui::Separator();
			if (ImGui::Button("Create Empty")) {
				App->object_manager->CreateGameObject(nullptr);
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("x10")) {
				for (int i = 0; i < 10; i++)
					App->object_manager->CreateGameObject(nullptr);
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("3D Object")) {
				/*if (ImGui::Selectable("Cube")) // TODO: Do a for loop or a ImGui::Combo
					App->object_manager->CreatePrimitive(Primitives::CUBE, Object_data());
				if (ImGui::Selectable("Tetrahedron"))
					App->object_manager->CreatePrimitive(Primitives::TETRAHEDRON, Object_data());
				if (ImGui::Selectable("Octahedron"))
					App->object_manager->CreatePrimitive(Primitives::OCTAHEDRON, Object_data());
				if (ImGui::Selectable("Dodecahedron"))
					App->object_manager->CreatePrimitive(Primitives::DODECAHEDRON, Object_data());
				if (ImGui::Selectable("Icosahedron"))
					App->object_manager->CreatePrimitive(Primitives::ICOSAHEDRON, Object_data());
				if (ImGui::Selectable("Sphere"))
					App->object_manager->CreatePrimitive(Primitives::SPHERE, Object_data());
				if (ImGui::Selectable("Hemisphere"))
					App->object_manager->CreatePrimitive(Primitives::HEMISPHERE, Object_data());
				if (ImGui::Selectable("Torus"))
					App->object_manager->CreatePrimitive(Primitives::TORUS, Object_data());
				if (ImGui::Selectable("Cone"))
					App->object_manager->CreatePrimitive(Primitives::CONE, Object_data());
				if (ImGui::Selectable("Cylinder"))
					App->object_manager->CreatePrimitive(Primitives::CYLINDER, Object_data());*/
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug Tools")) {
			if (ImGui::BeginMenu("Octree")) {
				if (ImGui::MenuItem("Scene")) {
					App->scene_intro->DebugOctree();
				}
				ImGui::MenuItem("Show Octree", NULL, &App->scene_intro->show_octree);

				ImGui::EndMenu();
			}

			ImGui::MenuItem("Show RayCast Mouse Picking", NULL, &App->scene_intro->show_mouse_raycast);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo", NULL, show_demo_window))
			{
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("Repository", "link"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Empty-Whisper/WhispEngine", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download Lastest", "link"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Empty-Whisper/WhispEngine/releases/latest", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug", "link"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Empty-Whisper/WhispEngine/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::MenuItem("About", "Control+Shift+A", &about->active);
			ImGui::EndMenu();

		}		
		ImGui::SameLine(ImGui::GetWindowWidth() - 90);
		HelpMarker("(?)Info", "Right click        - View around \nWASD + Right click - Move through viewport\nShift              - Speed up\nAlt + Right click  - Zoom\nScroll wheel       - Fast Zoom\nMiddle mouse       - Move X/Y viewport\nF                  - Focus object\nAlt + Left click   - Orbit object");


	}
	ImGui::EndMainMenuBar();

	return ret;
}

void ModuleGUI::OpenLoadWindow()
{
	char filename[MAX_PATH];

	char current_dir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, current_dir);

	std::string dir = std::string(current_dir) + "\\" + "Assets\\Scenes";

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Whisp Scene\0*.scene\0";
	ofn.lpstrFile = filename;
	ofn.lpstrInitialDir = dir.c_str();
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save new scene";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (GetOpenFileNameA(&ofn))
	{
		SetCurrentDirectoryA(current_dir);

		std::string file = App->file_system->GetFileFromPath(filename);
		//std::string path = App->file_system->GetRelativePathToAssets(filename);
		std::string path(filename);

		if (App->file_system->GetFormat(file.c_str()) != FileSystem::Format::SCENE) {
			LOG("File is not a scene file, cannot open");
		}
		else {
			if (!App->file_system->IsInDirectory(SCENE_A_FOLDER, file.c_str()))
				if (!App->file_system->IsInSubDirectory(ASSETS_FOLDER, file.c_str()))
					LOG("Scene file not in Assets folder, we recommend you to work in Assets folder");

			App->LoadScene(path.c_str());
		}
	}
	else
		SetCurrentDirectoryA(current_dir);
}

void ModuleGUI::OpenSaveWindow(bool create_empty)
{
	char filename[MAX_PATH];

	char current_dir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, current_dir);

	std::string dir = std::string(current_dir) + "\\" + "Assets\\Scenes";

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Whisp Scene\0*.scene\0";
	ofn.lpstrFile = filename;
	ofn.lpstrInitialDir = dir.c_str();
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save new scene";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (GetSaveFileNameA(&ofn))
	{
		SetCurrentDirectoryA(current_dir);

		std::string file = App->file_system->GetFileFromPath(filename);
		std::string path = App->file_system->GetRelativePathToAssets(filename);

		if (App->file_system->GetFormat(file.c_str()) != FileSystem::Format::SCENE) {
			file.append(".scene");
			path.append(".scene");
		}
		
		if (create_empty)
			App->scene_intro->CreateEmptyScene(path.c_str());
		else {
			App->scene_intro->scene_path.assign(path);
			App->scene_intro->SaveCurrentScene();
		}

		if (!App->file_system->IsInDirectory(SCENE_A_FOLDER, file.c_str()))
			if (!App->file_system->IsInSubDirectory(ASSETS_FOLDER, file.c_str()))
				LOG("Scene file not in Assets folder, we recommend you to work in Assets folder");
	}
	else
	{
		SetCurrentDirectoryA(current_dir);
	}
}

update_status ModuleGUI::PostUpdate()
{
	BROFILER_CATEGORY("GUI", Profiler::Color::Purple);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}


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

void ModuleGUI::Dockspace()
{
	BROFILER_CATEGORY("DockSpace", Profiler::Color::BlanchedAlmond);
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	// Seeting Docking to fit the window and preferences
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	/*if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;*/

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace BackGround", (bool*)0, window_flags);
	ImGui::PopStyleVar(3);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

void ModuleGUI::HelpMarker(const char* disable_txt,const char * shown_text)
{
	ImGui::TextDisabled(disable_txt);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextColored({ 0, 50, 50, 255 }, shown_text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

