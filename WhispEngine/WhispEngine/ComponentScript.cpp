#include "ComponentScript.h"
#include "Imgui/imgui.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "ModuleScripting.h"
#include "Lua/LuaBridge/LuaBridge.h"
#include <fstream>
#include "Imgui/imgui_internal.h"
#include "ModuleObjectManager.h"
#include "ModuleGUI.h"
#include "PanelResources.h"
#include "PanelScriptEditor.h"

#include "ModuleInput.h"

ComponentScript::ComponentScript(GameObject* parent) : Component(parent,ComponentType::SCRIPT)
{
}


ComponentScript::~ComponentScript()
{
	for (auto i = public_vars.begin(); i != public_vars.end(); i++) {
		delete (*i).second;
	}
}

void ComponentScript::Update()
{
	if (is_assigned && valid && App->IsGameRunning())
	{
		luabridge::setGlobal(App->scripting->GetState(), object, "object");
		luabridge::setGlobal(App->scripting->GetState(), object->transform, "transform");
		if (!public_vars.empty()) {
			SetInspectorVars();
		}
		
		App->scripting->ExecuteFunctionScript(script_path.c_str(), name.c_str(), (App->scripting->first_frame) ? ModuleScripting::Functions::START : ModuleScripting::Functions::UPDATE);
		
		lua_pop(App->scripting->GetState(), -1);
		lua_pop(App->scripting->GetState(), -1);
		if (!public_vars.empty())
			lua_pop(App->scripting->GetState(), -1);
	}
}

void ComponentScript::SetInspectorVars()
{
	luabridge::LuaRef t = luabridge::newTable(App->scripting->GetState());
	for (auto var = public_vars.begin(); var != public_vars.end(); var++) {
		switch ((*var).second->type)
		{
		case ComponentScript::TypeData::BOOL:
			t[(*var).first.c_str()] = static_cast<Property<bool>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::INT:
			t[(*var).first.c_str()] = static_cast<Property<int>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::FLOAT:
			t[(*var).first.c_str()] = static_cast<Property<float>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::NIL:
			t[(*var).first.c_str()] = static_cast<Property<int>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::TABLE:
			t[(*var).first.c_str()] = static_cast<Property<int>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::STRING:
			t[(*var).first.c_str()] = static_cast<Property<std::string>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::USERDATA:
			t[(*var).first.c_str()] = static_cast<Property<int>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::GAMEOBJECT:
			t[(*var).first.c_str()] = static_cast<Property<GameObject*>*>((*var).second)->data;
			break;
		case ComponentScript::TypeData::PREFAB:
			t[(*var).first.c_str()] = static_cast<Property<std::string>*>((*var).second)->data;
			break;
		default:
			break;
		}
	}
	luabridge::setGlobal(App->scripting->GetState(), t, "var");
}

void ComponentScript::OnInspector()
{
	ActiveImGui();
	ImGui::SameLine();
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
			if (ImGui::Button("Refresh")) {
				UpdateInspectorVars();
			}
			ImGui::SameLine();
			if (ImGui::Button("Change")) {
				OpenModalWindowsToLoadScript();
			}

			if (valid) {
				ImGui::Separator();

				DrawInspectorVars();
			}
		}
		else {
			SetScriptName();
		}
	}
}

void ComponentScript::SetScriptName()
{
	static char buffer[50];
	if (ImGui::InputText("", buffer, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
		// TODO: if(script already exists open modal window)
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
		App->gui->resources->RefreshFiles();
		App->gui->editor->SetFile(script_path.data());
	}
}

void ComponentScript::DrawInspectorVars()
{
	for (auto var = public_vars.begin(); var != public_vars.end(); var++) {
		switch ((*var).second->type)
		{
		case ComponentScript::TypeData::BOOL:
			ImGui::Checkbox((*var).first.c_str(), &static_cast<Property<bool>*>((*var).second)->data);
			break;
		case ComponentScript::TypeData::INT:
			if (!(*var).second->macros.empty()) {
				for (auto i = (*var).second->macros.begin(); i != (*var).second->macros.end(); ++i)
					switch (*i)
					{
					case ComponentScript::Macros::DRAG:
						ImGui::DragInt((*var).first.c_str(), &static_cast<Property<int>*>((*var).second)->data, 1, (int)(*var).second->macros_info["n1"], (int)(*var).second->macros_info["n2"]);
						break;
					case ComponentScript::Macros::INPUT:
						ImGui::InputInt((*var).first.c_str(), &static_cast<Property<int>*>((*var).second)->data);
						break;
					case ComponentScript::Macros::SLIDER:
						ImGui::SliderInt((*var).first.c_str(), &static_cast<Property<int>*>((*var).second)->data, (int)(*var).second->macros_info["n1"], (int)(*var).second->macros_info["n2"]);
						break;
					}
			}
			break;
		case ComponentScript::TypeData::FLOAT:
			if (!(*var).second->macros.empty()) {
				for (auto i = (*var).second->macros.begin(); i != (*var).second->macros.end(); ++i)
					switch (*i)
					{
					case ComponentScript::Macros::DRAG:
						ImGui::DragFloat((*var).first.c_str(), &static_cast<Property<float>*>((*var).second)->data, (*var).second->macros_info["n1"], (*var).second->macros_info["n2"]);
						break;
					case ComponentScript::Macros::INPUT:
						ImGui::InputFloat((*var).first.c_str(), &static_cast<Property<float>*>((*var).second)->data);
						break;
					case ComponentScript::Macros::SLIDER:
						ImGui::SliderFloat((*var).first.c_str(), &static_cast<Property<float>*>((*var).second)->data, (*var).second->macros_info["n1"], (*var).second->macros_info["n2"]);
						break;
					}
			}
			break;
		case ComponentScript::TypeData::NIL:
			break;
		case ComponentScript::TypeData::TABLE:
			break;
		case ComponentScript::TypeData::STRING:
			static char buff[50];
			strcpy(buff, static_cast<Property<std::string>*>((*var).second)->data.c_str());
			if (ImGui::InputText((*var).first.c_str(), buff, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
				static_cast<Property<std::string>*>((*var).second)->data.assign(buff);
			}
			break;
		case ComponentScript::TypeData::USERDATA:
			break;
		case ComponentScript::TypeData::GAMEOBJECT: {
			if (static_cast<Property<GameObject*>*>((*var).second)->data == nullptr)
				ImGui::InputText((*var).first.c_str(), "None", strlen("None"), ImGuiInputTextFlags_ReadOnly);
			else {
				char* name = (char*)static_cast<Property<GameObject*>*>((*var).second)->data->GetName();
				ImGui::InputText((*var).first.c_str(), name, strlen(name), ImGuiInputTextFlags_ReadOnly);
			}
			ImRect rect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
			if (ImGui::BeginDragDropTargetCustom(rect, ImGui::GetID("Hierarchy"))) { //Window
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_POINTER")) {
					static_cast<Property<GameObject*>*>((*var).second)->data = *(GameObject**)payload->Data;
				}
				ImGui::EndDragDropTarget();
			}
		}
			break;
		case ComponentScript::TypeData::PREFAB: {
			if (static_cast<Property<std::string>*>((*var).second)->data.empty())
				ImGui::InputText((*var).first.c_str(), "None", strlen("None"), ImGuiInputTextFlags_ReadOnly);
			else {
				std::string name = App->file_system->GetFileNameFromPath((char*)static_cast<Property<std::string>*>((*var).second)->data.c_str());
				ImGui::InputText((*var).first.c_str(), (char*)name.c_str(), name.length(), ImGuiInputTextFlags_ReadOnly);
			}
			ImRect rect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
			if (ImGui::BeginDragDropTargetCustom(rect, ImGui::GetID("Hierarchy"))) { //Window
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB")) {
					static_cast<Property<std::string>*>((*var).second)->data = App->gui->resources->file_dragdrop.c_str();
				}
				ImGui::EndDragDropTarget();
			}
		}
			break;
		default:
			break;
		}
	}
}

void ComponentScript::OpenModalWindowsToLoadScript()
{
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

void ComponentScript::UpdateInspectorVars()
{
	if (luaL_dofile(App->scripting->GetState(), script_path.c_str()) == 0) {
		luabridge::LuaRef ref = luabridge::getGlobal(App->scripting->GetState(), name.data());
		if (ref.isTable()) {
			ref = ref["Variables"];
			if (ref.isTable()) {
				std::ifstream is(script_path.c_str());
				std::string str;
				std::list<std::string> keys;
				std::map<std::string, std::string> macros;
				bool in_table = false;
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
									if (str.find("--") != std::string::npos) { // Found comment in public var
										if (str.find("[") != std::string::npos && str.find("]") != std::string::npos) { // Fond a macro
											auto m = str.begin() + str.find("[");
											std::string macro;
											while (*m != ']') {
												macro += *m;
												m++;
											}
											macro += ']';
											macros[word] = macro;
										}
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
				while (v != public_vars.end()) {
					if (std::find(keys.begin(), keys.end(), (*v).first) == keys.end()) {
						auto var = *v;
						v = public_vars.erase(v);
						delete var.second;
					}
					else
						v++;
				}
				for (auto k = keys.begin(); k != keys.end(); k++) {
					if (public_vars.find((*k).c_str()) == public_vars.end()) {
						auto r = ref[(*k).c_str()];

						if (r.isBool()) {
							Property<bool>* var = new Property<bool>(TypeData::BOOL, r.cast<bool>());
							public_vars[(*k).c_str()] = var;
						}
						if (r.isNil()) {
							if (macros.find((*k).c_str()) != macros.end()) {
								if (macros[(*k).c_str()].compare("[GameObject]") == 0) {
									Property<GameObject*>* var = new Property<GameObject*>(TypeData::GAMEOBJECT, nullptr);
									public_vars[(*k).c_str()] = var;
								}
								else if (macros[(*k).c_str()].compare("[Prefab]") == 0) {
									Property<std::string>* var = new Property<std::string>(TypeData::PREFAB, "");
									public_vars[(*k).c_str()] = var;
								}
							}
							else {
								Property<int>* var = new Property<int>(TypeData::NIL, 0);
								public_vars[(*k).c_str()] = var;
							}
						}
						if (r.isNumber()) {
							float f = r.cast<float>();
							if (ceilf(f) == f) { //integer
								Property<int>* var = new Property<int>(TypeData::INT, r.cast<int>());
								if (macros.find((*k)) != macros.end()) {
									if (macros[(*k)].find("[Slider") != std::string::npos) {
										var->macros.push_back(ComponentScript::Macros::SLIDER);
									}
									else if (macros[(*k).c_str()].find("[Drag") != std::string::npos) {
										var->macros.push_back(ComponentScript::Macros::DRAG);
									}
									else if (macros[(*k).c_str()].find("[Input") != std::string::npos) {
										var->macros.push_back(ComponentScript::Macros::INPUT);
									}
									auto par = macros[(*k).c_str()].find('(');
									if (par != std::string::npos) {
										auto it = macros[(*k).c_str()].begin() + par + 1;
										std::string n1, n2;
										while (*it != ',') {
											n1 += *it;
											it++;
										}
										while (*it == ' ' || *it == ',') {
											it++;
										}
										while (*it != ' ' && *it != ')') {
											n2 += *it;
											it++;
										}
										var->macros_info["n1"] = std::stof(n1);
										var->macros_info["n2"] = std::stof(n2);
									}
								}
								public_vars[(*k).c_str()] = var;
							}
							else {
								Property<float>* var = new Property<float>(TypeData::FLOAT, r.cast<float>());
								if (macros.find((*k)) != macros.end()) {
									if (macros[(*k)].find("[Slider") != std::string::npos) {
										var->macros.push_back(ComponentScript::Macros::SLIDER);
									}
									else if (macros[(*k).c_str()].find("[Drag") != std::string::npos) {
										var->macros.push_back(ComponentScript::Macros::DRAG);
									}
									else if (macros[(*k).c_str()].find("[Input") != std::string::npos) {
										var->macros.push_back(ComponentScript::Macros::INPUT);
									}
									auto par = macros[(*k).c_str()].find('(');
									if (par != std::string::npos) {
										auto it = macros[(*k).c_str()].begin() + par + 1;
										std::string n1, n2;
										while (*it != ',') {
											n1 += *it;
											it++;
										}
										while (*it == ' ' || *it == ',') {
											it++;
										}
										while (*it != ' ' && *it != ')') {
											n2 += *it;
											it++;
										}
										var->macros_info["n1"] = std::stof(n1);
										var->macros_info["n2"] = std::stof(n2);
									}
								}
								public_vars[(*k).c_str()] = var;
							}
						}
						if (r.isString()) {
							Property<std::string>* var = new Property<std::string>(TypeData::STRING, r.cast<std::string>());
							public_vars[(*k).c_str()] = var;
						}
						if (r.isTable()) {
							Property<int>* var = new Property<int>(TypeData::TABLE, 0);
							public_vars[(*k).c_str()] = var;
						}
						if (r.isUserdata()) {
							Property<int>* var = new Property<int>(TypeData::USERDATA, 0);
							public_vars[(*k).c_str()] = var;
						}
					}
				}
			}
		}
	}
	else {
		LOG("Cannot update inspector, Lua Error: %s", lua_tostring(App->scripting->GetState(), -1));
	}
}

void ComponentScript::SetScript(const char * path)
{
	is_assigned = true;
	if (App->file_system->Exists(path)) {
		script_path = path;
		name = App->file_system->GetFileNameFromPath(script_path.c_str());
		valid = true;
		UpdateInspectorVars();
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
	for (auto v = public_vars.begin(); v != public_vars.end(); v++) {
		nlohmann::json var;
		var["key"] = (*v).first;
		var["type"] = (*v).second->type;
		switch ((*v).second->type)
		{
		case ComponentScript::TypeData::BOOL:
			var["data"] = static_cast<Property<bool>*>((*v).second)->data;
			break;
		case ComponentScript::TypeData::INT:
			var["data"] = static_cast<Property<int>*>((*v).second)->data;
			break;
		case ComponentScript::TypeData::FLOAT:
			var["data"] = static_cast<Property<float>*>((*v).second)->data;
			break;
		case ComponentScript::TypeData::NIL:
			var["data"] = static_cast<Property<int>*>((*v).second)->data;
			break;
		case ComponentScript::TypeData::TABLE:
			var["data"] = static_cast<Property<int>*>((*v).second)->data;
			break;
		case ComponentScript::TypeData::STRING:
			var["data"] = static_cast<Property<std::string>*>((*v).second)->data.c_str();
			break;
		case ComponentScript::TypeData::USERDATA:
			var["data"] = static_cast<Property<int>*>((*v).second)->data;
			break;
		case ComponentScript::TypeData::GAMEOBJECT: {
			GameObject* o = static_cast<Property<GameObject*>*>((*v).second)->data;
			if (o != nullptr) var["data"] = o->GetUID();
			else			  var["data"] = 0;
		}
			break;
		case ComponentScript::TypeData::PREFAB:
			var["data"] = static_cast<Property<std::string>*>((*v).second)->data;
			break;
		default:
			break;
		}
		node["InspectorVars"].push_back(var);
	}
}

void ComponentScript::Load(const nlohmann::json & node)
{
	SetScript(node.value("script", "NONE").c_str());
	if (node.find("InspectorVars") != node.end()) {
		auto vars = node["InspectorVars"];
		for (auto v = vars.begin(); v != vars.end(); ++v) {
			if (public_vars.find((*v)["key"]) != public_vars.end()) {
				switch ((TypeData)(*v)["type"])
				{
				case ComponentScript::TypeData::BOOL:
					static_cast<Property<bool>*>(public_vars[(*v)["key"]])->data = (*v).value("data", false);
					break;
				case ComponentScript::TypeData::INT:
					static_cast<Property<int>*>(public_vars[(*v)["key"]])->data = (*v).value("data", 0);
					break;
				case ComponentScript::TypeData::FLOAT:
					static_cast<Property<float>*>(public_vars[(*v)["key"]])->data = (*v).value("data", 0.f);
					break;
				case ComponentScript::TypeData::NIL:
					static_cast<Property<int>*>(public_vars[(*v)["key"]])->data = (*v).value("data", 0);
					break;
				case ComponentScript::TypeData::TABLE:
					static_cast<Property<int>*>(public_vars[(*v)["key"]])->data = (*v).value("data", 0);
					break;
				case ComponentScript::TypeData::STRING:
					static_cast<Property<std::string>*>(public_vars[(*v)["key"]])->data.assign((*v).value("data", "none"));
					break;
				case ComponentScript::TypeData::USERDATA:
					static_cast<Property<int>*>(public_vars[(*v)["key"]])->data = (*v).value("data", 0);
					break;
				case ComponentScript::TypeData::GAMEOBJECT:
					static_cast<Property<GameObject*>*>(public_vars[(*v)["key"]])->data = App->object_manager->Find((uint64)(*v).value("data", (uint64)0));
					break;
				case ComponentScript::TypeData::PREFAB:
					static_cast<Property<std::string>*>(public_vars[(*v)["key"]])->data = (*v).value("data", "");
					break;
				default:
					break;
				}
			}
		}
	}
}
