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
		}
		else {
			static char buffer[50];
			if (ImGui::InputText("", buffer, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
				name.assign(buffer);
				title = name + " (Script)";
				script_path.append(name + ".lua");
				App->file_system->Copy("Assets/Internal/model.lua", script_path.data());
				is_assigned = true;
			}
		}
	}
}
