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

		if (!App->file_system->IsInDirectory(MODEL_AFOLDER, App->file_system->GetFileFromPath(path).data())) {
			new_path = (MODEL_AFOLDER + App->file_system->GetFileFromPath(path));
			if (!CopyFile(path, new_path.data(), FALSE)) {
				LOG("Failed to copy fbx in Assets folder, Error: %s", GetLastError());
			}
			else {
				path = new_path.data();
			}
		}

		// .meta -------------------------------------------------------------
		uint64_t meta = App->random->RandomGUID();
		char* meta_data = new char[sizeof(uint64_t)];
		memset(meta_data, meta, sizeof(uint64_t));
		App->file_system->SaveData(meta_data, std::string(path + std::string(".meta")).data(), sizeof(uint64_t));
		delete[] meta_data;
		// -------------------------------------------------------------------

		App->object_manager->SetSelected(nullptr);
		uint ticks = SDL_GetTicks(); //timer
		PerfTimer timer;

		aiNode *node = scene->mRootNode;

		HierarchyInfo info;
		std::string name = App->file_system->GetFileNameFromPath(path);
		uint header[2] = { node->mNumChildren, name.length() };

		uint size = CalculateHierarchyInfo(&info, node, scene) + sizeof(header) + name.length();

		char* data = new char[size];
		memset(data, 0, size);
		char* cursor = data;

		uint bytes = sizeof(header);
		memcpy(cursor, header, bytes);
		cursor += bytes;
		bytes = strlen(name.data());
		memcpy(cursor, name.data(), bytes);

		cursor += bytes;
		FillChildrenInfo(info, cursor);

		App->file_system->SaveData(data, std::string(MODEL_LFOLDER + name + std::string(".whispModel")).data(), size);

		delete[] data;

		aiReleaseImport(scene);
		LOG("Time to load FBX: %u", SDL_GetTicks() - ticks);
	}
	else
		LOG("Error loading scene: %s", scene == nullptr ? aiGetErrorString() : "The FBX has no meshes");

	return ret;
}

void ModelImporter::FillChildrenInfo(ModelImporter::HierarchyInfo &info, char * &cursor)
{
	uint bytes = 0;
	for (auto i = info.children.begin(); i != info.children.end(); ++i) {
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
		if (child_n->mNumMeshes == 1) {
			child.mesh_id = App->random->RandomGUID();
			App->importer->mesh->Import(child.mesh_id, scene->mMeshes[child_n->mMeshes[0]]);
		}
		else if (child_n->mNumMeshes > 1) {
			for (int i = 0; i < child_n->mNumMeshes; ++i) {
				HierarchyInfo child_mesh;
				child_mesh.mesh_id = App->random->RandomGUID();
				App->importer->mesh->Import(child_mesh.mesh_id, scene->mMeshes[child_n->mMeshes[i]]);
				child.children.push_back(child_mesh);
				size += sizeof(uint/*num_children*/) + sizeof(char/*has_mesh*/) + sizeof(uint64_t/*mesh_id*/);
			}
		}
		size += sizeof(uint/*num_children*/) + sizeof(char/*has_mesh*/) + sizeof(uint64_t/*mesh_id*/);

		if (child_n->mNumChildren > 0)
			size += CalculateHierarchyInfo(&child, child_n, scene);

		info->children.push_back(child);
	}
	

	return size;
}
