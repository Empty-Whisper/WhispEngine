#include "ModuleObjectManager.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "Brofiler/Brofiler.h"
#include "Imgui/ImGuizmo.h"
#include "PanelScene.h"

#include "PanelHierarchy.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleGUI.h"
#include "ModuleImport.h"
#include "ModuleRenderer3D.h"

#include "ModuleScripting.h"
#include "Lua/LuaBridge/LuaBridge.h"

ModuleObjectManager::ModuleObjectManager()
{
	name.assign("ObjectManager");
}


ModuleObjectManager::~ModuleObjectManager()
{
}

bool ModuleObjectManager::Start()
{
	root = new GameObject(nullptr);
	root->SetName("Root");
	//App->importer->Import("Assets/Textures/Checker.dds", );

	return true;
}

update_status ModuleObjectManager::Update()
{
	BROFILER_CATEGORY("GameObject Manager", Profiler::Color::MediumSpringGreen);
	glEnable(GL_LIGHTING);

	//MousePicking
	if (App->input->GetMouseButtonDown(1) && !App->input->GetKey(SDL_SCANCODE_LALT))
		MousePicking();

	//Update GO
	UpdateGameObject(root);

	//Camera
	App->camera->GetGameCamera()->DrawInsideFrustum();

	if (!to_change.empty()) {
		for (auto chng = to_change.begin(); chng != to_change.end(); chng++) {
			if (!(*chng).second->HasChild((*chng).first)) { // if the GameObject we want to change is parent of the other, we will leap that situation
				(*chng).second->Detach();
				(*chng).second->Attach((*chng).first);
			}
		}
		to_change.clear();
	}

	return UPDATE_CONTINUE;
}

void ModuleObjectManager::UpdateGameObject(GameObject* &obj)
{
	if (obj->IsActive()) {

		obj->Update();

		if (!obj->children.empty()) {
			for (auto i = obj->children.begin(); i != obj->children.end(); ++i) {
				UpdateGameObject(*i);
			}
		}
	}
}

bool ModuleObjectManager::CleanUp()
{
	delete root;

	return true;
}

GameObject * ModuleObjectManager::CreateGameObject(GameObject * parent)
{
	if (parent == nullptr)
		parent = root;

	GameObject* ret = new GameObject(parent);
	objects[ret->UID] = ret;
	
	return ret;
}

void ModuleObjectManager::DestroyGameObject(GameObject * obj)
{
	if (obj->parent != nullptr)
		obj->parent->children.erase(std::find(obj->parent->children.begin(), obj->parent->children.end(), obj));
	delete obj;
}

void ModuleObjectManager::ResetObjects()
{
	delete root;
	objects.clear();
	root = new GameObject(nullptr);
	root->SetName("Root");
}

GameObject * ModuleObjectManager::GetRoot() const
{
	return root;
}

void ModuleObjectManager::GetChildsFrom(GameObject* &obj, std::vector<GameObject*> &vector) const
{
	if (!obj->children.empty()) {

		for (auto i : obj->children) {
			vector.push_back(i);

			GetChildsFrom(i, vector);
		}
	}
}

const AABB ModuleObjectManager::GetMaxAABB(GameObject * obj, std::vector<GameObject*> &objects) const
{
	float3 max_point, min_point;
	max_point = min_point = float3::zero;
	GetChildsFrom(obj, objects);
	ComponentMesh* mesh = nullptr;
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		if ((*i)->TryGetComponent(ComponentType::MESH, (Component*&)mesh)) {
			AABB obj_aabb = mesh->GetAABB();
			if (max_point.x < obj_aabb.MaxX())
				max_point.x = obj_aabb.MaxX();
			if (max_point.y < obj_aabb.MaxY())
				max_point.y = obj_aabb.MaxY();
			if (max_point.z < obj_aabb.MaxZ())
				max_point.z = obj_aabb.MaxZ();

			if (min_point.x > obj_aabb.MinX())
				min_point.x = obj_aabb.MinX();
			if (min_point.y > obj_aabb.MinY())
				min_point.y = obj_aabb.MinY();
			if (min_point.z > obj_aabb.MinZ())
				min_point.z = obj_aabb.MinZ();
		}
	}

	return AABB(min_point, max_point);
}

GameObject * ModuleObjectManager::GetSelected() const
{
	return selected;
}

void ModuleObjectManager::SetSelected(GameObject * select)
{

	if (selected != select)
	{
		if (selected != nullptr)
			selected->Deselect();
		
		if (select != nullptr)
			select->Select();
		selected = select;
	}
}

void ModuleObjectManager::ChangeHierarchy(GameObject * dst, GameObject * src)
{
	to_change.emplace(std::pair<GameObject*, GameObject*>(dst, src));
}

void ModuleObjectManager::LuaRegister()
{
	using namespace luabridge;
	getGlobalNamespace(App->scripting->GetState())
		.beginClass<float3>("Vector3")
			.addConstructor<void(*) (const float&, const float&, const float&)>()
			.addConstructor<void(*) ()>()
			.addData("x", &float3::x, true)
			.addData("y", &float3::y, true)
			.addData("z", &float3::z, true)
			.addFunction("Normalize", &float3::Normalize)
			.addFunction("Normalized", &float3::Normalized)
			.addFunction("Magnitude", &float3::Length)
			.addFunction("sqrMagnitude", &float3::LengthSq)
			.addFunction("toString", &float3::ToString)
			.addFunction("__add", &float3::LSum)
			.addFunction("__sub", &float3::LSub)
			.addFunction("__mul", &float3::operator*)
			.addFunction("__div", &float3::operator/)
		.endClass()
		.beginNamespace("Vector3")
			//.addProperty("forward", &float3::unitX)
			//.addProperty("up", &float3::unitY)
			//.addProperty("right", &float3::unitZ)
			.addFunction("FromEuler", &Quat::FromEulerXYZ)
		.endNamespace()
		.beginClass<Quat>("Quaternion")
			.addConstructor<void(*) (const float&, const float&, const float&, const float&)>()
			.addData("w", &Quat::w)
			.addData("x", &Quat::x)
			.addData("y", &Quat::y)
			.addData("z", &Quat::z)
			.addFunction("Normalize", &Quat::Normalize)
			.addFunction("Normalized", &Quat::Normalized)
			.addFunction("ToEuler", &Quat::ToEulerXYZ)
			.addFunction("ToString", &Quat::ToString)
			.addFunction("SetFromAxisAngle", &Quat::SetFromAxisAngle)
		.endClass()
		.beginNamespace("Quaternion")
			.addFunction("FromEuler", &Quat::FromEulerXYZ)
			.addFunction("RotateAxisAngle", &Quat::RotateAxisAngle)
			.addFunction("RotateX", &Quat::RotateX)
			.addFunction("RotateY", &Quat::RotateY)
			.addFunction("RotateZ", &Quat::RotateZ)
		.endNamespace()
		.beginClass<GameObject>("GameObject")
			.addProperty("active", &GameObject::active)
			.addProperty("name", &GameObject::name)
			.addProperty("transform", &GameObject::transform)
			//.addFunction("addComponent", &GameObject::CreateComponent)
		.endClass()
		.beginClass<ComponentTransform>("transform")
			.addData("gameobject", &ComponentTransform::object, false)
			.addProperty("parent", &ComponentTransform::LGetParent)
			.addProperty("position", &ComponentTransform::GetPosition) //TODO: maybe it could be an own struct and set x, y and z as &position.x (READ ONLY)
			.addProperty("rotation", &ComponentTransform::GetRotation)
			.addProperty("scale", &ComponentTransform::GetScale)
			.addFunction("SetPositionv", &ComponentTransform::LSetPositionV)
			.addFunction("SetPosition3f", &ComponentTransform::LSetPosition3f)
			.addFunction("SetRotation", &ComponentTransform::LSetRotationQ)
			.addFunction("SetScale", &ComponentTransform::LSetScale3f)
			.addFunction("LookAt", &ComponentTransform::LLookAt)
			.addFunction("Find", &ComponentTransform::Find)
			.addProperty("ChildCount", &ComponentTransform::ChildCount)
			.addFunction("GetChild", &ComponentTransform::GetChild)
			.addFunction("IsChildOf", &ComponentTransform::IsChildOf)
			.addFunction("SetParent", &ComponentTransform::SetParent)
		.endClass();
}

void ModuleObjectManager::MousePicking()
{
	//Calculate Scene & Mouse vectors
	int window_x, window_y;
	SDL_GetWindowPosition(App->window->window, &window_x, &window_y);
	float2 window = {(float)window_x, (float)window_y};

	static ImVec2 scenePositionScreen;
	static ImVec2 scenePosition;
	static ImVec2 sceneSize;

	scenePositionScreen = App->gui->scene->GetPanelPos();
	scenePosition = { scenePositionScreen.x - window.x, scenePositionScreen.y - window.y };

	sceneSize = App->gui->scene->GetPanelSize();
	sceneSize = { scenePosition.x + sceneSize.x, scenePosition.y + sceneSize.y };

	float2 mousePos = {(float)App->input->GetMouseX(), (float)App->input->GetMouseY()};

	//Point Rect Formula
	if (mousePos.x <= sceneSize.x && mousePos.x >= scenePosition.x && mousePos.y < sceneSize.y && mousePos.y > scenePosition.y)
	{
		//Formula normalized vector
		float2 normalizedVector((mousePos.x - scenePosition.x) / (sceneSize.x - scenePosition.x), (mousePos.y - scenePosition.y) / (sceneSize.y - scenePosition.y));
		normalizedVector = { (normalizedVector.x * 2) - 1, 1 - (normalizedVector.y * 2) };

		//RayCast
		LineSegment ray_cast = App->camera->GetSceneCamera()->GetFrustum().UnProjectLineSegment(normalizedVector.x, normalizedVector.y);
		float distance = 1000.f;
		App->scene_intro->mouse_pick_0 = ray_cast.a;
		App->scene_intro->mouse_pick_1 = ray_cast.b;

		GameObject* hitted = nullptr;
		std::vector<GameObject*> all_game_objects;
		GetChildsFrom(root, all_game_objects);

		bool is_hitted = false;
		for (GameObject* iterator : all_game_objects)
		{
			float length; bool intersect;
			//Raycast will use LineSegment to clarify if an aabb intersects with him
			iterator->Raycast(ray_cast, intersect, length);

			if (intersect && length < distance)
			{
				distance = length;
				hitted = iterator;
				is_hitted = true;
			}
			
		}
		
		if (!is_hitted && selected != nullptr && !ImGuizmo::IsOver() && !App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN)
		{
			std::vector<GameObject*> selected_and_childs;
			selected_and_childs.push_back(selected);
			GetChildsFrom(root, selected_and_childs);

			for (std::vector<GameObject*>::iterator iterator = selected_and_childs.begin(); iterator != selected_and_childs.end();)
			{
				(*iterator)->Deselect();
				iterator = selected_and_childs.erase(iterator);
				SetSelected(nullptr);
			}
		}
		if (hitted != nullptr && !ImGuizmo::IsOver())
		{
			SetSelected(hitted);
			std::vector<GameObject*> selected_and_childs;
			GetChildsFrom(hitted, selected_and_childs);
		}
		
	}
}

bool ModuleObjectManager::SaveGameObjects(nlohmann::json & file)
{
	bool ret = true;

	ret = root->Save(file["GameObjects"]);

	return ret;
}

bool ModuleObjectManager::LoadGameObjects(const nlohmann::json & it)
{
	bool ret = true;

	DestroyGameObject(root);
	root = new GameObject(nullptr);
	auto object = *it.begin();
	if (it.size() == 1) {
		root->UID = object["UID"];

		for (auto i = object["Children"].cbegin(); i != object["Children"].cend(); i++) {
			LoadGameObject(*i, root);
		}
	}

	RefreshObjectsUIDMap();

	return ret;
}

bool ModuleObjectManager::LoadScripts(const nlohmann::json & it)
{
	auto object = *it.begin();
	if (it.size() == 1) {
		for (auto i = object["Children"].cbegin(); i != object["Children"].cend(); i++) {
			LoadScript(*i);
		}
	}
	return true;
}

bool ModuleObjectManager::LoadGameObject(const nlohmann::json & node, GameObject * parent)
{
	bool ret = true;

	GameObject* obj = CreateGameObject(parent);
	obj->SetName(node.value("name", "GameObject").c_str());
	obj->UID = node["UID"];
	obj->SetActive(node["active"]);
	obj->SetStatic(node.value("static", false));
	obj->GetComponent(ComponentType::TRANSFORM)->Load(node);

	for (auto i = node["Components"].begin(); i != node["Components"].end(); ++i) {
		obj->CreateComponent((*i).value("type", ComponentType::NONE))->Load(*i);
	}

	for (auto i = node["Children"].begin(); i != node["Children"].end(); ++i) {
		LoadGameObject(*i, obj);
	}

	return ret;
}

bool ModuleObjectManager::LoadScript(const nlohmann::json & node)
{
	bool ret = true;

	GameObject* obj = objects[node["UID"]];

	if (obj != nullptr) {
		for (auto i = node["Components"].begin(); i != node["Components"].end(); ++i) {
			if ((*i).value("type", ComponentType::NONE) == ComponentType::SCRIPT) {
				auto s = obj->GetComponent(ComponentType::SCRIPT); // Get All Scripts if there is more than one
				if (s != nullptr)
					s->Load(*i);
			}
		}
	}

	for (auto i = node["Children"].begin(); i != node["Children"].end(); ++i) {
		LoadScript(*i);
	}

	return ret;
}

void ModuleObjectManager::RefreshObjectsUIDMap()
{
	objects.clear();
	objects[root->UID] = root;
	for (auto i = root->children.begin(); i != root->children.end(); i++) {
		RefreshUIDMap(*i);
	}
}

void ModuleObjectManager::RefreshUIDMap(GameObject * obj)
{
	objects[obj->UID] = obj;
	for (auto i = obj->children.begin(); i != obj->children.end(); i++) {
		RefreshUIDMap(*i);
	}
}

//const char * ModuleObjectManager::PrimitivesToString(const Primitives prim)
//{
//	const char* name = nullptr;
//	switch (prim)
//	{
//	case Primitives::CUBE:
//		name = "Cube";
//		break;
//	case Primitives::TETRAHEDRON:
//		name = "Tetrahedron";
//		break;
//	case Primitives::OCTAHEDRON:
//		name = "Octahedron";
//		break;
//	case Primitives::DODECAHEDRON:
//		name = "Dodecahedron";
//		break;
//	case Primitives::ICOSAHEDRON:
//		name = "Icosahedron";
//		break;
//	case Primitives::SPHERE:
//		name = "Sphere";
//		break;
//	case Primitives::HEMISPHERE:
//		name = "Hemisphere";
//		break;
//	case Primitives::TORUS:
//		name = "Torus";
//		break;
//	case Primitives::CONE:
//		name = "Cone";
//		break;
//	case Primitives::CYLINDER:
//		name = "Cylinder";
//		break;
//	default:
//		LOG("Added more primitives than expected, add the missing primitives to the for");
//		break;
//	}
//
//	return name;
//}

void ModuleObjectManager::UpdateGuizmo()
{
	ImGuizmo::MODE guizmoApply = guizmoMode;

	if (gizmoOperation == ImGuizmo::OPERATION::SCALE)
		guizmoApply = ImGuizmo::MODE::WORLD;

	ChangeGuizmoOperation(gizmoOperation);

	float4x4 global_transform = ((ComponentTransform*)GetSelected()->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().Transposed();
	float4x4 last_transform;
	float matrix_properties[16];

	ImGuizmo::Manipulate(App->camera->GetSceneCamera()->GetViewMatrix().ptr(), App->camera->GetSceneCamera()->GetProjectionMatrix().ptr(), gizmoOperation, guizmoApply, global_transform.ptr(), matrix_properties);
	FillMatrix(last_transform, matrix_properties);
	
	std::vector<GameObject*> selected_and_childs;
	if (selected != nullptr)
	{
		selected_and_childs.push_back(selected);
		GetChildsFrom(selected, selected_and_childs);
	}

	for (GameObject* i : selected_and_childs)
	{
		if (ImGuizmo::IsUsing() && !App->camera->is_moving_camera && !App->object_manager->GetSelected()->IsStatic())
		{
			float4x4 position_matrix = float4x4::identity;
			float4x4 rotation_matrix = float4x4::identity;
			float4x4 scale_matrix = float4x4::identity;

			if (i != nullptr) {			
				ComponentMesh* mesh = nullptr;
				if (i->TryGetComponent(ComponentType::MESH, (Component*&)mesh)) {
					mesh->CalulateAABB_OBB();
				}
			}

			switch (gizmoOperation)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
			{
				position_matrix = last_transform * ((ComponentTransform*)i->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix();
				((ComponentTransform*)i->GetComponent(ComponentType::TRANSFORM))->SetGlobalMatrix(position_matrix);
			}
			break;

			case ImGuizmo::OPERATION::ROTATE:
			{
				rotation_matrix = last_transform * ((ComponentTransform*)i->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix();
				((ComponentTransform*)i->GetComponent(ComponentType::TRANSFORM))->SetGlobalMatrix(rotation_matrix);
			}
			break;
			case ImGuizmo::OPERATION::SCALE:
			{
				if (i == selected_and_childs[0]) //Only the selected, in other words the parent
				{
					std::vector<GameObject*> childs;
					childs.push_back(i);
					GetChildsFrom(i, childs);

					if (childs.size() > 0)
					{
						for (GameObject* iterator : childs) //Iterate all objects
						{
							float4x4 transform_changed_save = last_transform;
							last_transform = last_transform * transform_changed.Inverted();

							scale_matrix = last_transform * ((ComponentTransform*)iterator->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix();
							((ComponentTransform*)iterator->GetComponent(ComponentType::TRANSFORM))->SetGlobalMatrix(scale_matrix);
							transform_changed = transform_changed_save;


						}
					}
					else
					{
						float4x4 transform_changed_save = last_transform;
						last_transform = last_transform * transform_changed.Inverted();

						scale_matrix = last_transform * ((ComponentTransform*)i->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix();
						((ComponentTransform*)i->GetComponent(ComponentType::TRANSFORM))->SetGlobalMatrix(scale_matrix);

						transform_changed = transform_changed_save;
					}
				}
				

			}
			break;
			}
		}
		else
		{
			transform_changed = float4x4::identity;

		}
	}
}

void ModuleObjectManager::FillMatrix(float4x4 & matrix, float o[])
{
	matrix = float4x4(
		o[0], o[4], o[8], o[12],
		o[1], o[5], o[9], o[13],
		o[2], o[6], o[10], o[14],
		o[3], o[7], o[11], o[15]);
}

GameObject * ModuleObjectManager::Find(const uint64 & uid) const
{
	if (root->UID == uid)
		return root;
	if (objects.find(uid) != objects.end())
		return objects.at(uid);
	return nullptr;
}

void ModuleObjectManager::ChangeGuizmoOperation(ImGuizmo::OPERATION &gizmoOperation)
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		gizmoOperation = ImGuizmo::OPERATION::SCALE;
	}

}

void ModuleObjectManager::ChangeGuizmoMode(ImGuizmo::MODE & gizmoMode)
{
	this->guizmoMode = gizmoMode;
}


//Mesh_info * ModuleObjectManager::CreateMesh(const uint & n_vertex, const float * vertex, const uint & n_index, const uint * index, const float * normals, const float* texCoords, ComponentMesh* component)
//{
//	Mesh_info *mesh = new Mesh_info(component);
//
//	FillVertex(mesh, n_vertex, vertex);
//
//	FillIndex(mesh, n_index*3, index);
//
//	FillNormals(mesh, normals);
//
//	if (texCoords != nullptr)
//		FillTextureCoords(mesh, texCoords);
//
//	mesh->SetGLBuffers();
//
//	return mesh;
//}
//
//Mesh_info * ModuleObjectManager::CreateMesh(const aiMesh * mesh, ComponentMesh* component)
//{
//	Mesh_info *ret = new Mesh_info(component);
//
//	FillVertex(ret, mesh->mNumVertices, (float*)mesh->mVertices);
//
//	if (mesh->HasFaces())
//	{
//		FillIndex(ret, mesh->mNumFaces, mesh->mFaces);
//	}
//
//	if (mesh->HasNormals()) {
//
//		FillNormals(ret, (float*)mesh->mNormals);
//	}
//
//	if (mesh->HasTextureCoords(0)) {
//		FillTextureCoords(ret, (float*)mesh->mTextureCoords[0]);
//	}
//
//	ret->SetGLBuffers();
//
//	return ret;
//}
//
//void ModuleObjectManager::FillNormals(Mesh_info * ret, const float * normals)
//{
//	float magnitude = 0.3f; // To multiply normalized vectors
//
//	if (normals != nullptr) {
//		// Vertex Normals --------------------------------------------------
//		ret->vertex_normals.size = ret->vertex.size * 3;
//		ret->vertex_normals.data = new float[ret->vertex_normals.size];
//		memcpy(ret->vertex_normals.data, normals, sizeof(float) * ret->vertex_normals.size);
//	}
//
//	// Face Normals ----------------------------------------------------
//	ret->face_normals.size = ret->index.size * 2;
//	ret->face_normals.data = new float[ret->face_normals.size];
//	float* buffer = CalculateFaceNormals(ret->vertex.data, ret->face_normals.size, ret->index.size, ret->index.data, magnitude);
//	memcpy(ret->face_normals.data, buffer, ret->face_normals.size * sizeof(float));
//	delete[] buffer;
//}
//
float* ModuleObjectManager::CalculateFaceNormals(const float* vertex, const uint &n_face_normals, const uint &n_index, const uint* index, float magnitude)
{
	float* data = new float[n_face_normals];

	for (int k = 0; k < n_index / 3; k += 3) {
		float3 p1(vertex[index[k] * 3],	  vertex[index[k] * 3 + 1],		vertex[index[k] * 3 + 2]);
		float3 p2(vertex[index[k + 1] * 3], vertex[index[k + 1] * 3 + 1], vertex[index[k + 1] * 3 + 2]);
		float3 p3(vertex[index[k + 2] * 3], vertex[index[k + 2] * 3 + 1], vertex[index[k + 2] * 3 + 2]);

		data[k * 2] = (p1.x + p2.x + p3.x) / 3.f;
		data[k * 2 + 1] = (p1.y + p2.y + p3.y) / 3.f;
		data[k * 2 + 2] = (p1.z + p2.z + p3.z) / 3.f;

		float3 v1 = p2 - p1;
		float3 v2 = p3 - p1;

		float3 v_norm = (math::Cross(v1, v2)).Normalized();

		data[k * 2 + 3] = data[k * 2] + v_norm.x * magnitude;
		data[k * 2 + 4] = data[k * 2 + 1] + v_norm.y * magnitude;
		data[k * 2 + 5] = data[k * 2 + 2] + v_norm.z * magnitude;
	}

	return data;
}
//
//void ModuleObjectManager::FillIndex(Mesh_info * ret, const uint & n_index, const aiFace* faces)
//{
//	ret->index.size = n_index * 3 * 3;
//	ret->index.data = new uint[ret->index.size];
//
//	for (uint j = 0; j < n_index; ++j)
//	{
//		if (faces[j].mNumIndices != 3)
//		{
//			LOG("WARNING, geometry face with != 3 indices!");
//			ret->index.data[j * 3] = 0;
//			ret->index.data[j * 3 + 1] = 0;
//			ret->index.data[j * 3 + 2] = 0;
//		}
//		else
//		{
//			memcpy(&ret->index.data[j * 3], faces[j].mIndices, sizeof(uint) * 3);
//		}
//	}
//	LOG("New mesh with %i faces", ret->index.size / 6);
//}
//
//void ModuleObjectManager::FillIndex(Mesh_info * ret, const uint & n_index, const uint * index)
//{
//	ret->index.size = n_index * 3;
//	ret->index.data = new uint[ret->index.size];
//	memcpy(ret->index.data, index, sizeof(uint) * n_index);
//	LOG("New mesh with %i faces", ret->index.size / 3);
//}
//
//void ModuleObjectManager::FillVertex(Mesh_info * ret, const uint & n_vertex, const float* vertex)
//{
//	ret->vertex.size = n_vertex;
//	ret->vertex.data = new float[ret->vertex.size * 3];
//	memcpy(ret->vertex.data, vertex, sizeof(float) * ret->vertex.size * 3);
//	LOG("New mesh with %d vertex", ret->vertex.size);
//}
//
//void ModuleObjectManager::FillTextureCoords(Mesh_info * mesh, const float * textureCoords)
//{
//	mesh->tex_coords.size = mesh->vertex.size;
//	mesh->tex_coords.data = new float[mesh->tex_coords.size * 3];
//	memcpy(mesh->tex_coords.data, textureCoords, sizeof(float) * mesh->tex_coords.size * 3);
//}
//
//
//
//Mesh_info * ModuleObjectManager::CreateMeshPrimitive(const Primitives & type, ComponentMesh* component)
//{
//	Object_data data = Object_data();
//	par_shapes_mesh* prim = nullptr;
//
//	switch (type)
//	{
//	case Primitives::CUBE:
//		prim = par_shapes_create_cube();
//		break;
//	case Primitives::TETRAHEDRON:
//		prim = par_shapes_create_tetrahedron();
//		break;
//	case Primitives::OCTAHEDRON:
//		prim = par_shapes_create_octahedron();
//		break;
//	case Primitives::DODECAHEDRON:
//		prim = par_shapes_create_dodecahedron();
//		break;
//	case Primitives::ICOSAHEDRON:
//		prim = par_shapes_create_icosahedron();
//		break;
//	case Primitives::SPHERE:
//		//TODO Create a sphere with radious, rings and sectors, not by subdivisions https://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c/5989676#5989676
//		prim = par_shapes_create_subdivided_sphere(data.slices);
//		break;
//	case Primitives::HEMISPHERE:
//		prim = par_shapes_create_hemisphere(data.slices, data.rings);
//		break;
//	case Primitives::TORUS:
//		prim = par_shapes_create_torus(data.slices, data.rings, data.radius);
//		break;
//	case Primitives::CONE:
//		prim = par_shapes_create_cone(data.slices, data.rings);
//		break;
//	case Primitives::CYLINDER:
//		prim = par_shapes_create_cylinder(data.slices, data.rings);
//		break;
//	default:
//		LOG("Primitive not found to create. id: %i", (int)type);
//		break;
//	}
//
//	Mesh_info* mesh = CreateMesh(prim->npoints, prim->points, prim->ntriangles, prim->triangles, prim->normals, prim->tcoords, component);
//	par_shapes_free_mesh(prim);
//
//	return mesh;
//}
//
//bool ModuleObjectManager::CreatePrimitive(const Primitives & type, const Object_data &data)
//{
//	bool ret = true;
//
//	par_shapes_mesh* prim = nullptr;
//
//	switch (type)
//	{
//	case Primitives::CUBE:
//		prim = par_shapes_create_cube();
//		break;
//	case Primitives::TETRAHEDRON:
//		prim = par_shapes_create_tetrahedron();
//		break;
//	case Primitives::OCTAHEDRON:
//		prim = par_shapes_create_octahedron();
//		break;
//	case Primitives::DODECAHEDRON:
//		prim = par_shapes_create_dodecahedron();
//		break;
//	case Primitives::ICOSAHEDRON:
//		prim = par_shapes_create_icosahedron();
//		break;
//	case Primitives::SPHERE:
//		//TODO Create a sphere with radious, rings and sectors, not by subdivisions https://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c/5989676#5989676
//		prim = par_shapes_create_subdivided_sphere(data.slices);
//		break;
//	case Primitives::HEMISPHERE:
//		prim = par_shapes_create_hemisphere(data.slices, data.rings);
//		break;
//	case Primitives::TORUS:
//		prim = par_shapes_create_torus(data.slices, data.rings, data.radius);
//		break;
//	case Primitives::CONE:
//		prim = par_shapes_create_cone(data.slices, data.rings);
//		break;
//	case Primitives::CYLINDER:
//		prim = par_shapes_create_cylinder(data.slices, data.rings);
//		break;
//	default:
//		LOG("Primitive not found to create. id: %i", (int)type);
//		break;
//	}
//
//	par_shapes_translate(prim, data.pos.x, data.pos.y, data.pos.z);
//
//	if (data.rotate.axis[0] != 0 || data.rotate.axis[1] != 0 || data.rotate.axis[2] != 0) {
//		float mgn = std::sqrt(data.rotate.axis[0] * data.rotate.axis[0] + data.rotate.axis[1] * data.rotate.axis[1] + data.rotate.axis[2] * data.rotate.axis[2]); // normalize rotation axis
//		float rot[3] = { data.rotate.axis[0] / mgn,data.rotate.axis[1] / mgn ,data.rotate.axis[2] / mgn };
//		par_shapes_rotate(prim, data.rotate.angle, rot);
//	}
//
//	par_shapes_scale(prim, data.scale.x, data.scale.y, data.scale.z);
//
//	GameObject* obj = CreateGameObject(nullptr);
//	obj->SetName(PrimitivesToString(type));
//
//	ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(ComponentType::MESH);
//	mesh->mesh = CreateMesh(prim->npoints, prim->points, prim->ntriangles, prim->triangles, prim->normals, prim->tcoords, mesh);
//	
//	ComponentMaterial* mat = (ComponentMaterial*)obj->GetComponent(ComponentType::MATERIAL);
//	mat->SetFaceColor(data.face_color[0],data.face_color[1],data.face_color[2],1.f); 
//	mat->SetWireColor(data.wire_color[0], data.wire_color[1], data.wire_color[2], 1.f);
//
//	par_shapes_free_mesh(prim);
//
//	return ret;
//}
//
//void ModuleObjectManager::Demo()
//{
//	std::vector<par_shapes_mesh*> prim;
//	prim.resize((int)Primitives::MAX);
//	prim[0] = par_shapes_create_cube();
//	prim[1] = par_shapes_create_tetrahedron();
//	prim[2] = par_shapes_create_octahedron();
//	prim[3] = par_shapes_create_dodecahedron();
//	prim[4] = par_shapes_create_icosahedron();
//	prim[5] = par_shapes_create_subdivided_sphere(3);
//	prim[6] = par_shapes_create_hemisphere(10, 10);
//	prim[7] = par_shapes_create_torus(10, 10, 0.5f);
//	prim[8] = par_shapes_create_cone(10, 10);
//	prim[9] = par_shapes_create_cylinder(10, 10);
//
//	
//	int posx = -10;
//	for (auto i = prim.begin(); i != prim.end(); ++i) {
//		par_shapes_translate(*i, posx, 0.f, 3);
//
//		GameObject* obj = CreateGameObject(nullptr);
//		obj->SetName(PrimitivesToString((Primitives)std::distance(prim.begin(), i)));
//
//		ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(ComponentType::MESH);
//		mesh->mesh = CreateMesh((*i)->npoints, (*i)->points, (*i)->ntriangles, (*i)->triangles, (*i)->normals, (*i)->tcoords, mesh);
//
//		ComponentMaterial* mat = (ComponentMaterial*)obj->GetComponent(ComponentType::MATERIAL);
//
//		float rnd_color[4] = { App->random->Randomf(0.f,1.f), App->random->Randomf(0.f,1.f), App->random->Randomf(0.f,1.f), 1.f };
//
//		mat->SetFaceColor(&rnd_color[0]);
//
//		par_shapes_free_mesh((*i));
//		posx += 3;
//	}
//
//	prim.clear();	
//}
