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

	//  Input Shortcut Keys
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN)
	{
		show_console_window = !show_console_window;
	}
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN)
	{
		show_configuration_window = !show_configuration_window;
	}
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN)
	{
		show_style_window = !show_style_window;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				ret = update_status::UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "LShift+1"))
			{
				show_console_window = !show_console_window;
			}
			if (ImGui::MenuItem("Configuration", "LShift+2"))
			{
				show_configuration_window = !show_configuration_window;
			}
			if (ImGui::MenuItem("Style Editor", "LShift+3"))
			{
				show_style_window = !show_style_window;
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
			if (ImGui::MenuItem("About", NULL, show_about_window))
			{
				show_about_window = !show_about_window;
				
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	
	if (show_console_window)
	{
		MenuWindowConsole(&show_console_window);
	}

	if (show_configuration_window)
	{
		MenuWindowConfiguration(&show_configuration_window);
	}

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow();
	}

	if (show_style_window)
	{
		if (ImGui::Begin("Style editor"))
		{
			ImGui::ShowStyleEditor();
		}
		ImGui::End();
	}

	if (show_about_window)
	{
		MenuWindowAbout(&show_about_window);

		
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

bool ModuleGUI::MenuWindowAbout(bool* p_open)
{
	bool ret = true;
	if (ImGui::Begin("About Whisp Engine", p_open))
	{
		ImGui::Text("Version 0.1-alpha");
		ImGui::Columns(1);
		ImGui::NewLine();
		ImGui::Text("By Christian Martínez @christt105 and Marc Gálvez @optus23 for learning purposes.");
		ImGui::Text("Whisp Engine is licensed under the MIT LICENSE, see LICENSE for more information.");
		ImGui::NewLine();
		ImGui::Checkbox("Show MIT LICENSE", &show_mit_license_window);
		
			
		
	}
	ImGui::End();
	return ret;
}

bool ModuleGUI::MenuWindowConsole(bool * p_open)
{
	bool ret = true;
	if (ImGui::Begin("Console", p_open))
	{
		ImGui::Text("------------- Welcome to Whisp Engine -------------");
	
	}
	ImGui::End();
	
	return ret;
}

bool ModuleGUI::MenuWindowConfiguration(bool * p_open)
{
	bool ret = true;
	if (ImGui::Begin("Configuration", p_open, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenu("Options", p_open))
		{
			if (ImGui::MenuItem("Set Defaults"))
			{
			}
			if (ImGui::MenuItem("Load"))
			{
			}
			if (ImGui::MenuItem("Save"))
			{
			}
			ImGui::End();
		}
		if (ImGui::CollapsingHeader("Appliction"))
		{
			ImGui::InputText("Whisp Engine", "App Name", 0);
			ImGui::InputText("Empty Whisper", "Organitzation", 0);
			ImGui::SliderInt("Max FPS", &actual_fps, 0, 120);
			ImGui::Text("Limit Framerate: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", actual_fps);

			//TODO: Frame rate graphic
			//TODO: Memory Consumption graphic

			ImGui::Text("Total Reported Mem: "); ImGui::SameLine(); ImGui::Text("%i", total_reported_mem);
			ImGui::Text("Total Actual Mem: "); ImGui::SameLine(); ImGui::Text("%i", total_actual_mem);
			ImGui::Text("Peak Reported Mem: "); ImGui::SameLine(); ImGui::Text("%i", peak_reported_mem);
			ImGui::Text("Peak Actual Mem: "); ImGui::SameLine(); ImGui::Text("%i", peak_actual_mem);
			ImGui::Text("Accumulated Reported Mem: "); ImGui::SameLine(); ImGui::Text("%i", accumulated_reported_mem);
			ImGui::Text("Accumulated Actual Mem: "); ImGui::SameLine(); ImGui::Text("%i", accumulated_actual_mem);
			ImGui::Text("Accumulated Alloc Mem: "); ImGui::SameLine(); ImGui::Text("%i", accumulated_alloc_unit);
			ImGui::Text("Total Alloc Unit Mem: "); ImGui::SameLine(); ImGui::Text("%i", total_alloc_unity_count);
			ImGui::Text("PeakAlloc Unit Mem: "); ImGui::SameLine(); ImGui::Text("%i", peak_alloc_unit_count);


		}
		if (ImGui::CollapsingHeader("Window"))
		{		
		}
		if (ImGui::CollapsingHeader("File System"))
		{
		}
		if (ImGui::CollapsingHeader("Input"))
		{
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
		}
	}
	ImGui::End();

	return ret;
}

