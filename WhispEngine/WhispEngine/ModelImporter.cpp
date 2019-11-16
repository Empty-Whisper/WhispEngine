#include "ModelImporter.h"
#include "Application.h"
#include "ComponentTransform.h"
#include "MeshImporter.h"

#include "Brofiler/Brofiler.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


ModelImporter::ModelImporter()
{
}


ModelImporter::~ModelImporter()
{
}

bool ModelImporter::Import(const char * path)
{
	BROFILER_CATEGORY("Import FBX", Profiler::Color::Green);
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);
	LOG("Importing fbx with path: %s", path);

	if (scene != nullptr && scene->HasMeshes())
	{
		std::string new_path;

		if (!App->dummy_file_system->IsInSubDirectory(MODEL_A_FOLDER, App->dummy_file_system->GetFileFromPath(path).data())) {
			new_path = (MODEL_A_FOLDER + App->dummy_file_system->GetFileFromPath(path));
			if (!CopyFile(path, new_path.data(), FALSE)) {
				LOG("Failed to copy fbx in Assets folder, Error: %s", GetLastError());
			}
			else {
				path = new_path.data();
			}
		}

		// .meta -------------------------------------------------------------
		uint64_t meta = 0u;
		if (App->dummy_file_system->IsInDirectory(MODEL_A_FOLDER, (App->dummy_file_system->GetFileFromPath(path) + ".meta").data())) {
			if (App->dummy_file_system->IsMetaVaild((std::string(path) + ".meta").data())) {
				LOG("File %s already imported", App->dummy_file_system->GetFileFromPath(path).data());
				return false;
			}
			else {
				meta = App->dummy_file_system->GetMeta((std::string(path) + ".meta").data());
			}
		}
		else {
			meta = App->dummy_file_system->GenerateMetaFile((std::string(path) + ".meta").data());
		}
		// -------------------------------------------------------------------

		App->object_manager->SetSelected(nullptr);
		uint ticks = SDL_GetTicks(); //timer
		PerfTimer timer;

		aiNode *node = scene->mRootNode;

		//H: num_children | name_size | name

		//Obj: length_name | name | num_children | has_mesh | mesh_UID | 16xfloat transform

		HierarchyInfo info;
		std::string name = App->dummy_file_system->GetFileNameFromPath(path);
		uint header[2] = { node->mNumChildren, name.length() + 1 };

		uint size = CalculateHierarchyInfo(&info, node, scene) + sizeof(header) + name.length() + 1;

		char* data = new char[size];
		memset(data, 0, size);
		char* cursor = data;
		nlohmann::json file;
		
		uint bytes = sizeof(header);
		memcpy(cursor, header, bytes);
		cursor += bytes;
		bytes = name.length() + 1; // '\0'
		memcpy(cursor, name.c_str(), bytes);

		cursor += bytes;
		FillChildrenInfo(info, cursor, file[name.c_str()]);

		if (App->dummy_file_system->Exists(MODEL_L_FOLDER) == false) {
			App->dummy_file_system->CreateDir(MODEL_L_FOLDER);
		}
		//App->dummy_file_system->SaveData(data, size, std::string(MODEL_L_FOLDER + std::to_string(meta) + std::string(".whispModel")).data());
		App->dummy_file_system->SaveFile(std::string(MODEL_L_FOLDER + std::to_string(meta) + std::string(".whispModel")).data(), file);

		delete[] data;

		aiReleaseImport(scene);
		LOG("Time to load FBX: %u", SDL_GetTicks() - ticks);
	}
	else
		LOG("Error loading scene: %s", scene == nullptr ? aiGetErrorString() : "The FBX has no meshes");

	return ret;
}

bool ModelImporter::Load(const char * path)
{
	nlohmann::json data = App->dummy_file_system->OpenFile(path);
	if (data == nullptr) {
		LOG("Model %s not found", path);
		return false;
	}

	GameObject* container = App->object_manager->CreateGameObject(nullptr);
	if (data.is_object()) {
		nlohmann::json::iterator object = data.begin();
		container->SetName(object.key().c_str());
		if ((*object).is_array()) {
			for (nlohmann::json::iterator it = (*object).begin(); it != (*object).end(); it++) {
				CreateObjects(container, *it);
			}
		}
	}
	/*char* data = App->dummy_file_system->GetData(path);
	if (data == nullptr) {
		LOG("Model %s not found", path);
		return false;
	}

	char* cursor = data;

	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	GameObject* container = App->object_manager->CreateGameObject(nullptr);

	cursor += bytes;
	bytes = ranges[1];
	std::string name(cursor);
	container->SetName(name.c_str());

	cursor += bytes;
	for (int i = 0; i < ranges[0]; i++) {
		CreateObjects(container, cursor);
	}


	delete[] data;*/
	return true;
}

void ModelImporter::CreateObjects(GameObject * container, const nlohmann::json & data)
{
	uint bytes = 0;
	GameObject* child = App->object_manager->CreateGameObject(container);

	child->SetName(data["name"].get<std::string>().c_str());
	ComponentTransform* transform = (ComponentTransform*)child->GetComponent(ComponentType::TRANSFORM);
	transform->SetLocalMatrix(
		float3(data["position"][0],data["position"][1], data["position"][2]),
		Quat(data["rotation"][1], data["rotation"][2], data["rotation"][3], data["rotation"][0]),
		float3(data["scale"][0], data["scale"][1], data["scale"][2])
	);
	transform->CalculateGlobalMatrix();

	if (data.value("meshId", 0) != 0) {
		ComponentMesh* mesh = (ComponentMesh*)child->CreateComponent(ComponentType::MESH);
		mesh->mesh = new Mesh_info();
		mesh->mesh->component = mesh;

		App->importer->mesh->Load(data["meshId"], mesh->mesh);
	}

	if (data.find("children") != data.end()) {
		for (nlohmann::json::const_iterator it = data["children"].begin(); it != data["children"].end(); it++) {
			CreateObjects(child, *it);
		}
	}

	//bytes = sizeof(uint);
	//uint n_characters = 0u;
	//memcpy(&n_characters, cursor, bytes);
	//cursor += bytes;

	//bytes = n_characters;
	////std::string name(cursor);
	////child->SetName(name.c_str());
	//cursor += bytes;

	//uint num_children;
	//bytes = sizeof(uint);
	//memcpy(&num_children, cursor, bytes);
	//cursor += bytes;

	//bytes = sizeof(uint64_t);
	//if (cursor[0] == 1) {
	//	ComponentMesh* mesh = (ComponentMesh*)child->CreateComponent(ComponentType::MESH);
	//	uint64_t mesh_uid = 0;
	//	memcpy(&mesh_uid, cursor+1, bytes);

	//	mesh->mesh = new Mesh_info();
	//	mesh->mesh->component = mesh;

	//	App->importer->mesh->Load(mesh_uid, mesh->mesh);
	//}
	//	cursor += bytes + 1;

	//	bytes = sizeof(float) * 16;
	//	ComponentTransform* transform = (ComponentTransform*)child->GetComponent(ComponentType::TRANSFORM);
	//	float4x4 matrix = float4x4::identity;
	//	matrix.Set((float*)cursor);
	//	transform->SetLocalMatrix(matrix);
	//	transform->CalculeLocalMatrix();
	//	transform->CalculateGlobalMatrix();
	//	cursor += bytes;

	//for (int i = 0; i < num_children; i++) {
	//	CreateObjects(child, cursor);
	//}
}

void ModelImporter::FillChildrenInfo(ModelImporter::HierarchyInfo &info, char* &cursor, nlohmann::json & file)
{
	uint bytes = 0;
	for (auto i = info.children.begin(); i != info.children.end(); ++i) {
		nlohmann::json object;

		object["name"] = (*i).name.c_str();

		if ((*i).mesh_id > 0u)
			object["meshId"] = (*i).mesh_id;

		object["position"] = { (*i).position.x, (*i).position.y, (*i).position.z };
		object["rotation"] = { (*i).rotation.x, (*i).rotation.y, (*i).rotation.z, (*i).rotation.w};
		object["scale"]	   = { (*i).scale.x, (*i).scale.y, (*i).scale.z };

		if ((*i).children.size() > 0) {
			FillChildrenInfo(*i, cursor, object["children"]);
		}
		file.push_back(object);
		
		/*bytes = sizeof(uint);
		uint n_characters = (*i).name.length();
		memcpy(cursor, &n_characters, bytes);
		cursor += bytes;

		bytes = (*i).name.length();
		memcpy(cursor, (*i).name.c_str(), bytes);
		cursor += bytes;


		bytes = sizeof(uint);
		uint n_children = (*i).children.size();
		memcpy(cursor, &n_children, bytes);
		cursor += bytes;

		bytes = sizeof(char);
		char has_mesh = (*i).mesh_id == 0 ? 0 : 1;
		memcpy(cursor, &has_mesh, bytes);
		cursor += bytes;

		bytes = sizeof(uint64_t);
		memcpy(cursor, &(*i).mesh_id, bytes);
		cursor += bytes;

		bytes = sizeof(float) * 16;
		memcpy(cursor, (*i).transform, bytes);
		cursor += bytes;*/

	}
}

uint ModelImporter::CalculateHierarchyInfo(HierarchyInfo * info, const aiNode * node, const aiScene* scene)
{
	uint size = 0;
	for (int i = 0; i < node->mNumChildren; i++) {
		aiNode* child_n = node->mChildren[i];
		HierarchyInfo child;
		child.parent = info;
		child.name.assign(child_n->mName.C_Str());

		aiVector3D pos, scale; aiQuaternion rot;
		child_n->mTransformation.Decompose(scale, rot, pos);
		child.position.Set(pos.x, pos.y, pos.z);
		child.rotation.Set(rot.w, rot.x, rot.y, rot.z);
		child.scale.Set(scale.x, scale.y, scale.z);
		//memcpy(child.transform, &node->mTransformation, sizeof(float) * 16);
		

		if (child_n->mNumMeshes == 1) {
			child.mesh_id = App->random->RandomGUID();
			App->importer->mesh->Import(child.mesh_id, scene->mMeshes[child_n->mMeshes[0]], scene);
		}
		else if (child_n->mNumMeshes > 1) {
			for (int i = 0; i < child_n->mNumMeshes; ++i) {
				HierarchyInfo child_mesh;
				child_mesh.mesh_id = App->random->RandomGUID();
				child_mesh.name.assign(scene->mMeshes[child_n->mMeshes[i]]->mName.C_Str());
				//memcpy(child_mesh.transform, &node->mTransformation, sizeof(float) * 16);
				App->importer->mesh->Import(child_mesh.mesh_id, scene->mMeshes[child_n->mMeshes[i]], scene);
				child.children.push_back(child_mesh);
				size += sizeof(uint/*num_children*/) + sizeof(char/*has_mesh*/) + sizeof(uint64_t/*mesh_id*/) + sizeof(uint/*n_characters*/) + child_mesh.name.length() + 1 + sizeof(float) * 16;
			}
		}
		size += sizeof(uint/*num_children*/) + sizeof(char/*has_mesh*/) + sizeof(uint64_t/*mesh_id*/) + sizeof(uint/*n_characters*/) + child.name.length() + 1 + sizeof(float) * 16;

		if (child_n->mNumChildren > 0)
			size += CalculateHierarchyInfo(&child, child_n, scene);

		info->children.push_back(child);
	}

	return size;
}
