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
		if (App->dummy_file_system->IsInDirectory(MODEL_A_FOLDER, (App->dummy_file_system->GetFileFromPath(path) + ".meta").data())) {
			if (App->dummy_file_system->IsMetaVaild((std::string(path) + ".meta").data())) {
				LOG("File %s already imported", App->dummy_file_system->GetFileFromPath(path).data());
				return false;
			}
		}
		uint64_t meta = App->dummy_file_system->GenerateMetaFile((std::string(path) + ".meta").data());
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

		uint bytes = sizeof(header);
		memcpy(cursor, header, bytes);
		cursor += bytes;
		bytes = name.length() + 1; // '\0'
		memcpy(cursor, name.c_str(), bytes);

		cursor += bytes;
		FillChildrenInfo(info, cursor);

		if (App->dummy_file_system->Exists(MODEL_L_FOLDER) == false) {
			App->dummy_file_system->CreateDir(MODEL_L_FOLDER);
		}
		App->dummy_file_system->SaveData(data, size, std::string(MODEL_L_FOLDER + std::to_string(meta) + std::string(".whispModel")).data());

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
	char* data = App->dummy_file_system->GetData(path);
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


	delete[] data;
	return true;
}

void ModelImporter::CreateObjects(GameObject * container, char * &cursor)
{
	uint bytes = 0;
	GameObject* child = App->object_manager->CreateGameObject(container);

	bytes = sizeof(uint);
	uint n_characters = 0u;
	memcpy(&n_characters, cursor, bytes);
	cursor += bytes;

	bytes = n_characters;
	std::string name(cursor);
	child->SetName(name.c_str());
	cursor += bytes;

	uint num_children;
	bytes = sizeof(uint);
	memcpy(&num_children, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(uint64_t);
	if (cursor[0] == 1) {
		ComponentMesh* mesh = (ComponentMesh*)child->CreateComponent(ComponentType::MESH);
		uint64_t mesh_uid = 0;
		memcpy(&mesh_uid, cursor+1, bytes);

		mesh->mesh = new Mesh_info();
		mesh->mesh->component = mesh;

		App->importer->mesh->Load(mesh_uid, mesh->mesh);
	}
		cursor += bytes + 1;

	for (int i = 0; i < num_children; i++) {
		CreateObjects(child, cursor);
	}
}

void ModelImporter::FillChildrenInfo(ModelImporter::HierarchyInfo &info, char* &cursor)
{
	uint bytes = 0;
	for (auto i = info.children.begin(); i != info.children.end(); ++i) {
		bytes = sizeof(uint);
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

		if ((*i).children.size() > 0) {
			FillChildrenInfo(*i, cursor);
		}
	}
}

void ModelImporter::LoadNode(aiNode * node, GameObject * parent, const aiScene * scene)
{
	for (int i = 0; i < node->mNumChildren; ++i) {
		aiNode* child = node->mChildren[i];

		GameObject* obj = App->object_manager->CreateGameObject(parent);
		obj->SetName(child->mName.C_Str());
		LOG("Created %s GameObject", obj->GetName());

		ComponentTransform *transform = (ComponentTransform*)obj->GetComponent(ComponentType::TRANSFORM);
		aiVector3D position, scale;
		aiQuaternion rotation;
		child->mTransformation.Decompose(scale, rotation, position);

		transform->SetPosition(position.x, position.y, position.z);
		transform->SetRotation(rotation.w, rotation.x, rotation.y, rotation.z);
		// FBX exporters have some options that will change the scale of the models, be sure you export your models in Apply Scale FBX All mode

		//scale *= 0.01f;
		//scale /= std::max(std::max(scale.x, scale.y),scale.z); 
		transform->SetScale(scale.x, scale.y, scale.z);

		transform->CalculeLocalMatrix();
		transform->CalculateGlobalMatrix();

		if (child->mNumMeshes == 1) {
			ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(ComponentType::MESH);
			aiMesh* amesh = scene->mMeshes[child->mMeshes[0]];
			mesh->mesh = App->object_manager->CreateMesh(amesh);

			aiMaterial* aimaterial = scene->mMaterials[amesh->mMaterialIndex];
			aiString path;
			aimaterial->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
			LOG("Diffuse texture found: %s", path.C_Str());
			ComponentMaterial* material = (ComponentMaterial*)obj->GetComponent(ComponentType::MATERIAL);
			material->SetTexture(App->importer->ImportTexture(path.C_Str()));

		}
		else {
			for (int j = 0; j < child->mNumMeshes; ++j) {
				GameObject * child_m = App->object_manager->CreateGameObject(obj);

				ComponentMesh* mesh = static_cast<ComponentMesh*>(child_m->CreateComponent(ComponentType::MESH));
				aiMesh* amesh = scene->mMeshes[child->mMeshes[j]];
				mesh->mesh = App->object_manager->CreateMesh(amesh);

				child_m->SetName(amesh->mName.C_Str());
			}
		}

		if (child->mNumChildren > 0) {
			LoadNode(child, obj, scene);
		}
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
		if (child_n->mNumMeshes == 1) {
			child.mesh_id = App->random->RandomGUID();
			App->importer->mesh->Import(child.mesh_id, scene->mMeshes[child_n->mMeshes[0]], scene);
		}
		else if (child_n->mNumMeshes > 1) {
			for (int i = 0; i < child_n->mNumMeshes; ++i) {
				HierarchyInfo child_mesh;
				child_mesh.mesh_id = App->random->RandomGUID();
				child_mesh.name.assign(scene->mMeshes[child_n->mMeshes[i]]->mName.C_Str());
				App->importer->mesh->Import(child_mesh.mesh_id, scene->mMeshes[child_n->mMeshes[i]], scene);
				child.children.push_back(child_mesh);
				size += sizeof(uint/*num_children*/) + sizeof(char/*has_mesh*/) + sizeof(uint64_t/*mesh_id*/) + sizeof(uint/*n_characters*/) + child_mesh.name.length()+1;
			}
		}
		size += sizeof(uint/*num_children*/) + sizeof(char/*has_mesh*/) + sizeof(uint64_t/*mesh_id*/) + sizeof(uint/*n_characters*/) + child.name.length()+1;

		if (child_n->mNumChildren > 0)
			size += CalculateHierarchyInfo(&child, child_n, scene);

		info->children.push_back(child);
	}

	return size;
}
