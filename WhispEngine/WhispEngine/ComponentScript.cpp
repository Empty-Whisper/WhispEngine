#include "ComponentScript.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "ModuleScripting.h"
#include "Lua/LuaBridge/LuaBridge.h"

ComponentScript::ComponentScript(GameObject* parent) : Component(parent,ComponentType::SCRIPT)
{
}


ComponentScript::~ComponentScript()
{
}

void ComponentScript::Update()
{
	if (is_assigned)
	{
		luabridge::setGlobal(App->scripting->GetState(), object, "object");
		luabridge::setGlobal(App->scripting->GetState(), object->transform, "transform");
		App->scripting->ExecuteFunctionScript(script_path.c_str(), name.c_str(), "Update");
		lua_pop(App->scripting->GetState(), -1);
		lua_pop(App->scripting->GetState(), -1);
	}
}

void ComponentScript::OnInspector()
{
	if (ImGui::CollapsingHeader(title.data(), ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::BeginPopupContextItem("Mesh")) {
			if (ImGui::Button("Delete")) {
				object->DeleteComponent(this);
			}
			ImGui::EndPopup();
		}
		if (is_assigned) {
			if (valid)
				ImGui::Text("Script\t%s", name.data());
			else {
				ImGui::Text("Script\t"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s", name.data());
			}
			ImGui::SameLine();
			if (ImGui::Button("Change")) {
				char filename[MAX_PATH];

				char current_dir[MAX_PATH];
				GetCurrentDirectoryA(MAX_PATH, current_dir);

				std::string dir = std::string(current_dir) + "\\" + "Assets\\Scripts";

				OPENFILENAME ofn;
				ZeroMemory(&filename, sizeof(filename));
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
				ofn.lpstrFilter = "Whisp Script\0*.lua\0";
				ofn.lpstrFile = filename;
				ofn.lpstrInitialDir = dir.c_str();
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrTitle = "Load Script";
				ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

				if (GetOpenFileNameA(&ofn))
				{
					SetCurrentDirectoryA(current_dir);

					if (App->file_system->GetFormat(filename) != FileSystem::Format::LUA) {
						LOG("File is not a script file, cannot open");
					}
					else {
						if (!App->file_system->IsInDirectory(SCENE_A_FOLDER, filename))
							if (!App->file_system->IsInSubDirectory(ASSETS_FOLDER, filename))
								LOG("Script file not in Assets folder, we recommend you to work in Assets folder");

						SetScript(filename);
					}
				}
				else
					SetCurrentDirectoryA(current_dir);
			}
		}
		else {
			static char buffer[50];
			if (ImGui::InputText("", buffer, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
				name.assign(buffer);
				title = name + " (Script)";
				script_path.append(name + ".lua");
				App->file_system->Copy("Assets/Internal/model.lua", script_path.data());
				valid = true;

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

void ComponentScript::SetScript(const char * path)
{
	is_assigned = true;
	if (App->file_system->Exists(path)) {
		script_path = path;
		name = App->file_system->GetFileNameFromPath(script_path.c_str());
		valid = true;
	}
	else {
		script_path = "NONE";
		name = "NONE";
		valid = false;
	}
	title = name + " (Script)";
}

void ComponentScript::Save(nlohmann::json & node)
{
	node["script"] = script_path.c_str();
}

void ComponentScript::Load(const nlohmann::json & node)
{
	SetScript(node.value("script", "NONE").c_str());
}
