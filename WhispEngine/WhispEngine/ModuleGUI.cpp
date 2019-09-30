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


ModuleGUI::ModuleGUI(bool enable_true) :Module(enable_true)
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

	panels.push_back(config = new PanelConfiguration(true, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_2));
	panels.push_back(about = new PanelAbout(true, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_LCTRL, SDL_SCANCODE_A));
	panels.push_back(console = new PanelConsole(true, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_1));

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	
	ImGui::NewFrame();

	//  Input Shortcut Keys
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKeyDown(SDL_SCANCODE_1))
	{
		show_console_window = !show_console_window;
	}
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKeyDown(SDL_SCANCODE_3))
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
			if (ImGui::MenuItem("Quit"))
			{
				ret = update_status::UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{		
			ImGui::MenuItem("Console", "LShift+1", &show_console_window);
			ImGui::MenuItem("Console", "LShift+1", &console->active);
			ImGui::MenuItem("Configuration", "LShift+2", &config->active);
			ImGui::MenuItem("Style Editor", "LShift+3", &show_style_window);
			
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
			/*if (ImGui::MenuItem("About", NULL, show_about_window))
			{
				show_about_window = !show_about_window;
				
			}*/
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	for (auto i = panels.begin(); i != panels.end(); ++i) {
		if ((*i)->IsShortCutPressed()) {
			(*i)->ChangeActive();
		}
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

//bool ModuleGUI::MenuWindowAbout()
//{
//	bool ret = true;
//	if (ImGui::Begin("About Whisp Engine", &show_about_window))
//	{
//		ImGui::Text("Version 0.1-alpha");
//		ImGui::Separator();
//		ImGui::NewLine();
//		ImGui::Text("By Christian Mart�nez @christt105 and Marc G�lvez @optus23 for learning purposes.");
//		ImGui::Text("Whisp Engine is licensed under the MIT LICENSE, see LICENSE for more information.");
//		ImGui::NewLine();
//		if(ImGui::Button("Github @christ105")) 	ShellExecuteA(NULL, "open", "https://github.com/christt105", NULL, NULL, SW_SHOWNORMAL);
//		ImGui::SameLine();
//		if(ImGui::Button("Github @optus23")) 	ShellExecuteA(NULL, "open", "https://github.com/optus23", NULL, NULL, SW_SHOWNORMAL);
//		ImGui::NewLine();
//		ImGui::Checkbox("Show MIT LICENSE", &checkbox_mit_license_window);
//		if (checkbox_mit_license_window)
//		{
//			ImGui::SetWindowSize(ImVec2(580, 530));
//			ImGui::NewLine(); ImGui::NewLine();
//			ImGui::Text("MIT LICENSE");	ImGui::NewLine();
//			ImGui::Text("Copyright (c) 2019 Empty-Whisper"); ImGui::NewLine();
//			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :"); ImGui::NewLine();
//			ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software."); ImGui::NewLine();
//			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE."); ImGui::NewLine();
//		}
//		else
//		{
//			ImGui::SetWindowSize(ImVec2(580, 200));
//		}		
//	}
//	ImGui::End();
//	return ret;
//}

bool ModuleGUI::MenuWindowConsole()
{
	bool ret = true;

	ImGui::Begin("Console", &show_console_window);
	ImGui::TextUnformatted(buffer.begin());
	if (log_new_line)
		ImGui::SetScrollHereY(1.0f);
	log_new_line = false;
	ImGui::End();
	
	return ret;
}
bool ModuleGUI::MenuWindowAbout()
{
	bool ret = true;
	if (ImGui::Begin("About Whisp Engine", &show_about_window))
	{
		ImGui::Text("Version 0.1-alpha");
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("By Christian Mart�nez @christt105 and Marc G�lvez @optus23 for learning purposes.");
		ImGui::Text("Whisp Engine is licensed under the MIT LICENSE, see LICENSE for more information.");
		ImGui::NewLine();
		if(ImGui::Button("Github @christ105")) 	ShellExecuteA(NULL, "open", "https://github.com/christt105", NULL, NULL, SW_SHOWNORMAL);
		ImGui::SameLine();
		if(ImGui::Button("Github @optus23")) 	ShellExecuteA(NULL, "open", "https://github.com/optus23", NULL, NULL, SW_SHOWNORMAL);
		ImGui::NewLine();
		ImGui::Checkbox("Show MIT LICENSE", &checkbox_mit_license_window);
		if (checkbox_mit_license_window)
		{
			ImGui::SetWindowSize(ImVec2(580, 530));
			ImGui::NewLine(); ImGui::NewLine();
			ImGui::Text("MIT LICENSE");	ImGui::NewLine();
			ImGui::Text("Copyright (c) 2019 Empty-Whisper"); ImGui::NewLine();
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :"); ImGui::NewLine();
			ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software."); ImGui::NewLine();
			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE."); ImGui::NewLine();
		}
		else
		{
			ImGui::SetWindowSize(ImVec2(580, 200));
		}
			
		
	}
	ImGui::End();
	return ret;
}
