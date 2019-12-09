#include "ComponentScript.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "FileSystem.h"

ComponentScript::ComponentScript(GameObject* parent) : Component(parent,ComponentType::SCRIPT)
{
}


ComponentScript::~ComponentScript()
{
}

void ComponentScript::OnInspector()
{
	if (ImGui::CollapsingHeader(title.data(), ImGuiTreeNodeFlags_DefaultOpen)) {
		if (is_assigned) {
			ImGui::Text("Script\t%s", name.data());
			char* file = App->file_system->GetTextFile(script_path.data());
			if (file != nullptr) {
				LOG("%s", file);
				delete[] file;
			}
		}
		else {
			static char buffer[50];
			if (ImGui::InputText("", buffer, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
				name.assign(buffer);
				title = name + " (Script)";
				script_path.append(name + ".lua");
				App->file_system->Copy("Assets/Internal/model.lua", script_path.data());

				char* file = App->file_system->GetTextFile(script_path.data());
				std::string sfile(file);
				delete[] file;

				while (sfile.find("Model") != std::string::npos) {
					sfile.replace(sfile.find("Model"), 5, name.c_str());
				}
				while (sfile.find('\r') != std::string::npos) {
					sfile.replace(sfile.find('\r'), 1, "");
				}

				App->file_system->SaveTextFile(sfile.data(), script_path.c_str());
				is_assigned = true;
			}
		}
	}
}
