#include "Application.h"
#include "Globals.h"
#include "ModuleGUI.h"

#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "GPUdetect/DeviceId.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"


ModuleGUI::ModuleGUI(bool enable_true) :Module(enable_true)
{
	uint vendor_id, device_id;
	Uint64 mp_buget, mb_usage, mb_available, vmb_reserved;
	std::wstring brand;

	if (getGraphicsDeviceInfo(&vendor_id, &device_id, &brand, &mp_buget, &mb_usage, &mb_available, &vmb_reserved))
	{
		hardware.gpu_vendor = vendor_id;
		hardware.gpu_device = device_id;
		sprintf_s(hardware.gpu_brand, 250, "%S", brand.c_str());
		hardware.vram_mb_budget = float(mp_buget) / (1024.f * 1024.f);
		hardware.vram_mb_usage = float(mb_usage) / (1024.f * 1024.f);
		hardware.vram_mb_available = float(mb_available) / (1024.f * 1024.f);
		hardware.vram_mb_reserved = float(vmb_reserved) / (1024.f * 1024.f);
	}
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	SDL_VERSION(&sdl_version);
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
		MenuWindowConsole();
	}

	if (show_configuration_window)
	{
		MenuWindowConfiguration();
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
		MenuWindowAbout();
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

void ModuleGUI::Log(const char * str)
{
	buffer.appendf(str);
	log_new_line = true;
}

bool ModuleGUI::MenuWindowAbout()
{
	bool ret = true;
	if (ImGui::Begin("About Whisp Engine"))
	{
		ImGui::Text("Version 0.1-alpha");
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("By Christian Martínez @christt105 and Marc Gálvez @optus23 for learning purposes.");
		ImGui::Text("Whisp Engine is licensed under the MIT LICENSE, see LICENSE for more information.");
		ImGui::NewLine();
		ImGui::Checkbox("Show MIT LICENSE", &show_mit_license_window);
		if (show_mit_license_window)
		{
			ImGui::SetWindowSize(ImVec2(580, 485));
			ImGui::NewLine(); ImGui::NewLine();
			ImGui::Text("MIT LICENSE");	ImGui::NewLine();
			ImGui::Text("Copyright (c) 2019 Empty-Whisper"); ImGui::NewLine();
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :"); ImGui::NewLine();
			ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software."); ImGui::NewLine();
			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE."); ImGui::NewLine();

		}
		else
		{
			ImGui::SetWindowSize(ImVec2(580, 150));
		}
			
		
	}
	ImGui::End();
	return ret;
}

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

bool ModuleGUI::MenuWindowConfiguration()
{
	bool ret = true;
	if (ImGui::Begin("Configuration", &show_configuration_window, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Set Defaults"))
			{
				// TODO: Load Default data from JSON
			}
			if (ImGui::MenuItem("Load"))
			{
				// TODO: Load Saved data from JSON
			}
			if (ImGui::MenuItem("Save"))
			{
				// TODO: Save data from JSON

			}
			ImGui::End();
		}
		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::InputText("App Name", TITLE, 0);
			ImGui::InputText("Organitzation", ORGANIZATION, 0);
			
			if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
			{
				App->framerate_cap = max_fps;
			}
			ImGui::Text("Limit Framerate: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", max_fps);

			//ImGui::Text("%f", App->prev_last_sec_frame_count);

			FillVectorFPS();
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_reg[fps_reg.size() - 1]);
			ImGui::PlotHistogram("framerate", &fps_reg[0], fps_reg.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			//TODO: MS graphic
			//TODO: Memory Consumption graphic

			ImGui::Text("Total Reported Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.total_reported_mem);
			ImGui::Text("Total Actual Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.total_actual_mem);
			ImGui::Text("Peak Reported Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.peak_reported_mem);
			ImGui::Text("Peak Actual Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.peak_actual_mem);
			ImGui::Text("Accumulated Reported Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.accumulated_reported_mem);
			ImGui::Text("Accumulated Actual Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.accumulated_actual_mem);
			ImGui::Text("Accumulated Alloc Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.accumulated_alloc_unit);
			ImGui::Text("Total Alloc Unit Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.total_alloc_unity_count);
			ImGui::Text("PeakAlloc Unit Mem: "); ImGui::SameLine(); ImGui::Text("%i", config.peak_alloc_unit_count);


		}
		if (ImGui::CollapsingHeader("Window"))
		{		

		}
		if (ImGui::CollapsingHeader("File System"))
		{
			// TODO: Wait for the next class indications
		}
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW,"%i, %i", App->input->GetMouseX(), App->input->GetMouseY());
			ImGui::Text("Mouse Motion: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "%i,%i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			ImGui::Text("Mouse Wheel: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "%i", App->input->GetMouseWheel());
			ImGui::Separator();

			ImGui::BeginChild("Buffer");
			ImGui::TextUnformatted(App->input->text_buffer.begin());
			if (App->input->auto_scroll)
			{
				ImGui::SetScrollHereY(1.0f);
				App->input->auto_scroll = false;
			}
			ImGui::EndChild();

				
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			std::string sdl_version_string = std::to_string(sdl_version.major) + "." + std::to_string(sdl_version.minor) + "." + std::to_string(sdl_version.patch);
			ImGui::Text("SDL Version: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, (sdl_version_string.c_str()));
			ImGui::Separator();

			std::string sdl_cpu_string = std::to_string(SDL_GetCPUCount()) + " (Cache: " + std::to_string(SDL_GetCPUCacheLineSize()) + "Kb)";
			ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, (sdl_cpu_string.c_str()));

			std::string sdl_ram_string = std::to_string(SDL_GetSystemRAM()) + "Mb";
			ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, (sdl_ram_string.c_str()));

			ImGui::Text("Caps: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, FindCapsHardware().c_str());
			ImGui::Separator();

			FindVRAMHardware();
			ImGui::Text("GPU: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "vendor %u device %u", hardware.gpu_vendor, hardware.gpu_device);
			ImGui::Text("Brand: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, (hardware.gpu_brand));
			ImGui::Text("VRAM Budget: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "%.1f Mb", hardware.vram_mb_budget);
			ImGui::Text("VRAM Usage: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "%.1f Mb", hardware.vram_mb_usage);
			ImGui::Text("VRAM Available: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "%.1f Mb", hardware.vram_mb_available);
			ImGui::Text("VRAM Reserved: "); ImGui::SameLine(); ImGui::TextColored(IMGUI_COLOR_YELLOW, "%.1f Mb", hardware.vram_mb_reserved);
		}
	}
	ImGui::End();

	return ret;
}

bool ModuleGUI::FillVectorFPS()
{
	bool ret = true;

	static uint count = 0;
	if (count >= MAX_FPS_COUNT)
	{
		for (uint i = 0; i < MAX_FPS_COUNT - 1; ++i)
		{
			fps_reg[i] = fps_reg[i + 1];
		}
		fps_reg[count - 1] = App->prev_last_sec_frame_count;

	}
	else
	{
		++count;
		if (fps_reg.size() < MAX_FPS_COUNT)
		{
			for (uint i = 0; i < MAX_FPS_COUNT; ++i)
			{
				fps_reg.push_back(i);
			}
		}
	}

	return ret;
}

bool ModuleGUI::FillVectorMS()
{
	bool ret = true;
	return ret;
}

bool ModuleGUI::FindVRAMHardware()
{
	bool ret = true;
	Uint64 mb_budget, mb_usage, mb_available, mb_reserved;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &mb_budget, &mb_usage, &mb_available, &mb_reserved))
	{
		hardware.vram_mb_budget = float(mb_budget) / (1024.f * 1024.f);
		hardware.vram_mb_usage = float(mb_usage) / (1024.f * 1024.f);
		hardware.vram_mb_available = float(mb_available) / (1024.f * 1024.f);
		hardware.vram_mb_reserved = float(mb_reserved) / (1024.f * 1024.f);
	}
	
	return ret;
}



std::string ModuleGUI::FindCapsHardware()
{
	std::string caps;

	if (SDL_Has3DNow())
		caps.append("3DNow, ");
	if (SDL_HasAVX())
		caps.append("AVX, ");
	if (SDL_HasAVX2())
		caps.append("AVX2, ");
	if (SDL_HasMMX())
		caps.append("MMX, ");
	if (SDL_HasRDTSC())
		caps.append("RDTSC, ");
	if (SDL_HasSSE())
		caps.append("SSE, ");
	if (SDL_HasSSE2())
		caps.append("SSE2, ");
	if (SDL_HasSSE3())
		caps.append("SSE3, ");
	if (SDL_HasSSE41())
		caps.append("SSE41, ");
	if (SDL_HasSSE42())
		caps.append("SSE42, ");

	return caps.c_str();

}



