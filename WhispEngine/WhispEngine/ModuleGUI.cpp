#include "Application.h"
#include "Globals.h"
#include "ModuleGUI.h"

#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"


ModuleGUI::ModuleGUI(Application * app, bool enable_true) :Module(app, enable_true)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_VERSION));

	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				ret = update_status::UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "Shift+1"))
			{
				show_console_window = !show_console_window;
			}
			if (ImGui::MenuItem("Configuration", "Shift+2"))
			{
				show_configuration_window = !show_configuration_window;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Demo"))
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
			if (ImGui::MenuItem("About"))
			{
				show_about_window = !show_about_window;
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	
	if (show_console_window)
	{
		if (ImGui::Begin("Console"))
		{

		}
		ImGui::End();
	}

	if (show_configuration_window)
	{
		if (ImGui::Begin("Configuration"))
		{
			
		}
		ImGui::End();
	}

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow();
	}

	if (show_about_window)
	{
		if (ImGui::Begin("About Whisp Engine"))
		{
			
			ImGui::SetWindowSize(ImVec2(700, 140));
			ImGui::Text("Version 0.1-alpha");
			ImGui::NewLine();
			ImGui::Text("By Christian Martínez @christt105 and Marc Gálvez @optus23 for learning purposes.");
			ImGui::Text("Whisp Engine is licensed under the MIT LICENSE, see LICENSE for more information.");
			//ImGui::Checkbox("Build Information",);
		}
		ImGui::End();
	}
	

	return ret;
}

update_status ModuleGUI::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}
