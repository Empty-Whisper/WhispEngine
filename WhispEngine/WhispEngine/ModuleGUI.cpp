#include "Application.h"
#include "Globals.h"
#include "ModuleGUI.h"

#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl2.h"


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
	ImGui_ImplOpenGL2_Init();

	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Demo")) {
			show_demo_window = !show_demo_window;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Close")) {
			ret = update_status::UPDATE_STOP;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (show_demo_window) {
		ImGui::ShowDemoWindow();
	}
	
	return ret;
}

update_status ModuleGUI::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}
