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
		if (App->dummy_file_system->IsInSubDirectory(MODEL_A_FOLDER, (App->dummy_file_system->GetFileFromPath(path) + ".meta").data())) {
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

		HierarchyInfo info;
		std::string name = App->dummy_file_system->GetFileNameFromPath(path);

		CalculateHierarchyInfo(&info, node, scene);

		nlohmann::json file;
		
		FillChildrenInfo(info, file[name.c_str()]);

		if (App->dummy_file_system->Exists(MODEL_L_FOLDER) == false) {
			App->dummy_file_system->CreateDir(MODEL_L_FOLDER);
		}
		
		App->dummy_file_system->SaveFile(std::string(MODEL_L_FOLDER + std::to_string(meta) + std::string(".whispModel")).data(), file);

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
	
	return true;
}

void ModelImporter::CreateObjects(GameObject * container, const nlohmann::json & data)
{
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
}

void ModelImporter::FillChildrenInfo(ModelImporter::HierarchyInfo &info, nlohmann::json & file)
{
	for (auto i = info.children.begin(); i != info.children.end(); ++i) {
		nlohmann::json object;

		object["name"] = (*i).name.c_str();

		if ((*i).mesh_id > 0u)
			object["meshId"] = (*i).mesh_id;

		object["position"] = { (*i).position.x, (*i).position.y, (*i).position.z };
		object["rotation"] = { (*i).rotation.x, (*i).rotation.y, (*i).rotation.z, (*i).rotation.w};
		object["scale"]	   = { (*i).scale.x, (*i).scale.y, (*i).scale.z };

		if ((*i).children.size() > 0) {
			FillChildrenInfo(*i, object["children"]);
		}
		
		file.push_back(object);
	}
}

void ModelImporter::CalculateHierarchyInfo(HierarchyInfo * info, const aiNode * node, const aiScene* scene)
{
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
		

		if (child_n->mNumMeshes == 1) {
			child.mesh_id = App->random->RandomGUID();
			App->importer->mesh->Import(child.mesh_id, scene->mMeshes[child_n->mMeshes[0]], scene);
		}
		else if (child_n->mNumMeshes > 1) {
			for (int j = 0; j < child_n->mNumMeshes; ++j) {
				HierarchyInfo child_mesh;
				child_mesh.mesh_id = App->random->RandomGUID();
				child_mesh.name.assign(scene->mMeshes[child_n->mMeshes[j]]->mName.C_Str());
				App->importer->mesh->Import(child_mesh.mesh_id, scene->mMeshes[child_n->mMeshes[j]], scene);
				child.children.push_back(child_mesh);
			}
		}

		if (child_n->mNumChildren > 0)
			CalculateHierarchyInfo(&child, child_n, scene);

		info->children.push_back(child);
	}
}
