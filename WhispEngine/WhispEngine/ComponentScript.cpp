#include "ComponentScript.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "ModuleScripting.h"
#include "Lua/LuaBridge/LuaBridge.h"
#include <fstream>

#include "ModuleInput.h"

ComponentScript::ComponentScript(GameObject* parent) : Component(parent,ComponentType::SCRIPT)
{
}


ComponentScript::~ComponentScript()
{
}

void ComponentScript::Update()
{
	if (is_assigned && valid)
	{
		luabridge::setGlobal(App->scripting->GetState(), object, "object");
		luabridge::setGlobal(App->scripting->GetState(), object->transform, "transform");
		//luabridge::LuaRef ref = luabridge::getGlobal(App->scripting->GetState(), name.data());
		luabridge::LuaRef t = luabridge::newTable(App->scripting->GetState());
		if (public_vars.empty())
			t["a"] = 5;
		else
			t["a"] = public_vars["a"].value;
		luabridge::setGlobal(App->scripting->GetState(), t, "var");
		/*if (ref.isTable()) {
			if (ref["Variables"].isTable()) {
				ref["Variables"]["a"] = 2;
				luabridge::setGlobal(App->scripting->GetState(), ref, name.c_str());
			}
		}*/
		/*if (ref.isTable()) {
			ref = ref["Variables"];
			if (ref.isTable()) {
				for (auto var = public_vars.begin(); var != public_vars.end(); var++) {
					auto r = ref[(*var).first.c_str()];
					r = (*var).second.value;
				}
				luabridge::setGlobal(App->scripting->GetState(), ref, (name + ".Variables").c_str());
			}
		}*/
		App->scripting->ExecuteFunctionScript(script_path.c_str(), name.c_str(), "Update");
		lua_pop(App->scripting->GetState(), -1);
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
			if (valid) {
				ImGui::Text("Script\t%s", name.data());
			}
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

			if (valid) {
				ImGui::Separator();

				for (auto var = public_vars.begin(); var != public_vars.end(); var++) {
					ImGui::SliderFloat((*var).first.c_str(), &(*var).second.value, 0.f, 10.f);
				}
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
	if (App->input->GetKeyDown(SDL_SCANCODE_A)) {
		luabridge::LuaRef ref = luabridge::getGlobal(App->scripting->GetState(), name.data());
		if (ref.isTable()) {
			ref = ref["Variables"];
			if (ref.isTable()) {
				std::ifstream is(script_path.c_str());
				std::string str;
				std::list<std::string> keys;
				bool in_table = false;
				int name_of_keys = 0;
				while (std::getline(is, str))
				{
					if (in_table) {
						if (str.find("=") != std::string::npos) { // All public variables must be initialized
							for (auto i = str.begin(); i != str.end(); i++) {
								if (std::isalpha(*i)) {
									auto j = i;
									std::string word;
									while (*j != ' ') {
										word += *j;
										j++;
									}
									keys.push_back(word);
									break;
								}
								if ((*i) == '-' && (*(i + 1)) == '-')
									break; // lua comment
							}
						}
						if (str.find("}") != std::string::npos)
							break;
					}
					else {
						if (str.find("Variables") != std::string::npos) {
							in_table = true;
						}
					}
				}
				auto v = public_vars.begin();
				while(v != public_vars.end()) {
					if (std::find(keys.begin(), keys.end(), (*v).first) == keys.end())
						v = public_vars.erase(v);
					else
						v++;
				}
				for (auto k = keys.begin(); k != keys.end(); k++) {
					LOG("variable name: %s", (*k).c_str());
					auto r = ref[(*k).c_str()];

					PublicVar var;

					if (r.isBool()) {
						LOG("is bool");
						var.type = "bool";
					}
					if (r.isFunction()) {
						LOG("is function");
						var.type = "function";
					}
					if (r.isNil()) {
						LOG("is nil");
						var.type = "nil";
					}
					if (r.isNumber()) {
						LOG("is number");
						var.type = "number";
						var.value = r.cast<float>();
					}
					if (r.isString()) {
						LOG("is string");
						var.type = "string";
					}
					if (r.isTable()) {
						LOG("is table");
						var.type = "table";
					}
					if (r.isUserdata()) {
						LOG("is userdata");
						var.type = "userdata";
					}
					public_vars[(*k).c_str()] = var;
				}

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
		luabridge::LuaRef ref = luabridge::getGlobal(App->scripting->GetState(), name.data());
		if (ref.isTable()) {
			ref = ref["Variables"];
			if (ref.isTable()) {
				LOG("string: %s", ref.tostring().c_str());
				LOG("length: %i", ref.length());
			}
		}
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
