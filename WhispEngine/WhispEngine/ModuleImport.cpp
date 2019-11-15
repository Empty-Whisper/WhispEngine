#include "ModuleImport.h"

#include "Application.h"
#include "ModuleObjectManager.h"

#include "ComponentTransform.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Brofiler/Brofiler.h"

#include "ModelImporter.h"
#include "MaterialImporter.h"
#include "MeshImporter.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleImport::ModuleImport()
{
	name.assign("Importer");
}


ModuleImport::~ModuleImport()
{
}

bool ModuleImport::Start()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	model		= new ModelImporter();
	material	= new MaterialImporter();
	mesh		= new MeshImporter();

	// Charge logo texture
	//logo_txt = ImportTexture("Assets/logo.png");
	//logo_txt->visible_on_inspector = false;

	return true;
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	delete model;
	delete material;
	delete mesh;

	return true;
}

bool ModuleImport::Import(const char * path)
{
	bool ret = false;
	switch (App->dummy_file_system->GetFormat(path))
	{
	case FileSystem::Format::JSON:
		break;
	case FileSystem::Format::DDS:
	case FileSystem::Format::PNG:
	case FileSystem::Format::JPG:
		ret = material->Import(path);
		break;
	case FileSystem::Format::FBX:
		ret = model->Import(path);
		break;
	case FileSystem::Format::MODEL:
		ret = model->Load(path);
		break;
	case FileSystem::Format::META: {
		char* f_uid = App->dummy_file_system->GetData(path);
		if (f_uid == nullptr) {
			LOG("Failed to open meta file, trying to reload asset...");
			std::string s_path(path);
			s_path.erase(s_path.end() - 5, s_path.end());
			Import(s_path.c_str());
			f_uid = App->dummy_file_system->GetData(path);
			if (f_uid == nullptr) {
				LOG("Failed another time to load meta file, aborting loading");
				return false;
			}
		}
		uint64_t uid = 0u;
		memcpy(&uid, f_uid, sizeof(uint64_t));

		delete[] f_uid;

		if (uid != 0u) {
			std::string s_path(path);
			s_path.erase(s_path.end() - 5, s_path.end());
			switch (App->dummy_file_system->GetFormat(s_path.c_str())) {
			case FileSystem::Format::FBX:
				if (App->dummy_file_system->Exists((MODEL_L_FOLDER + std::to_string(uid) + ".whispModel").c_str()) == false) {
					LOG("Model referenced in meta does not exists, recreating .meta...");
					//App->dummy_file_system->RemoveFile(path);
					std::string s_path(path);
					s_path.erase(s_path.end() - 5, s_path.end());
					if (Import(s_path.c_str()))
						Import(path);
				}
				else {
					ret = model->Load((MODEL_L_FOLDER + std::to_string(uid) + ".whispModel").c_str());
				}
				break;
			case FileSystem::Format::JPG:
			case FileSystem::Format::PNG:
			case FileSystem::Format::DDS:
				if (App->dummy_file_system->Exists((MATERIAL_L_FOLDER + std::to_string(uid) + ".dds").c_str()) == false) {
					LOG("Texture referenced in meta does not exists, recreating .meta...");
					//App->dummy_file_system->RemoveFile(path);
					std::string s_path(path);
					s_path.erase(s_path.end() - 5, s_path.end());
					if (Import(s_path.c_str()))
						Import(path);
				}
				else {
					ret = material->Load((MATERIAL_L_FOLDER + std::to_string(uid) + ".dds").c_str());
				}
				break;
			}
		}
	}
		break;
	default:
		LOG("Failed to load %s. Format not setted", path);
		break;
	}
	return ret;
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
			//material->SetTexture(App->importer->ImportTexture(path.C_Str()));

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