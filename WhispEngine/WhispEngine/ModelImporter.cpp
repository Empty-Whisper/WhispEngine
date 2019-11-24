#include "ModelImporter.h"
#include "Application.h"
#include "ComponentTransform.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "ModuleImport.h"

#include "ModuleObjectManager.h"
#include "ModuleResources.h"

#include "Brofiler/Brofiler.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "ResourceModel.h"


ModelImporter::ModelImporter()
{
}


ModelImporter::~ModelImporter()
{
}

uint64 ModelImporter::Import(const char * path)
{
	BROFILER_CATEGORY("Import FBX", Profiler::Color::Green);
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);
	LOG("Importing fbx with path: %s", path);

	if (scene != nullptr && scene->HasMeshes())
	{
		bool want_to_generate_meta = true;
		std::string new_path;

		if (!App->dummy_file_system->IsInSubDirectory(ASSETS_FOLDER, App->dummy_file_system->GetFileFromPath(path).data())) {
			if (App->dummy_file_system->Exists(MODEL_A_FOLDER) == false)
				App->dummy_file_system->CreateDir(MODEL_A_FOLDER);
			new_path = (MODEL_A_FOLDER + App->dummy_file_system->GetFileFromPath(path));
			if (!CopyFile(path, new_path.data(), FALSE)) {
				LOG("Failed to copy fbx in Assets folder", path);
			}
			else {
				path = new_path.data();
			}
		}

		std::vector<uint64> uid_meshes, uid_materials;

		// .meta -------------------------------------------------------------
		uint64_t meta = 0u;
		if (App->dummy_file_system->IsInSubDirectory(ASSETS_FOLDER, (App->dummy_file_system->GetFileFromPath(path) + ".meta").data())) {
			if (App->dummy_file_system->IsMetaVaild((std::string(path) + ".meta").data())) {
				LOG("File %s already imported", App->dummy_file_system->GetFileFromPath(path).data());
				meta = App->dummy_file_system->GetUIDFromMeta((std::string(path) + ".meta").data());
				Resource* res = App->resources->CreateResource(Resource::Type::MODEL, meta);
				res->SetFile(path);
				res->SetResourcePath((MODEL_L_FOLDER + std::to_string(meta) + ".whispModel").c_str());
				
				//TODO: Load Resource
				char* data = App->dummy_file_system->GetData((std::string(path) + ".meta").data());
				if (data != nullptr) {
					char* cursor = data + sizeof(uint64);

					uint n_meshes = 0u;
					memcpy(&n_meshes, cursor, sizeof(uint));
					cursor += sizeof(uint);
					for (int i = 0; i < n_meshes; i++) {
						uint64 uid = 0u;
						memcpy(&uid, cursor, sizeof(uint64));
						Resource* res = App->resources->CreateResource(Resource::Type::MESH, uid);
						res->SetFile(path);
						res->SetResourcePath((MESH_L_FOLDER + std::to_string(uid) + ".whispMesh").c_str());
						cursor += sizeof(uint64);
					}
					
					/*uint n_materials = 0u;
					std::memcpy(&n_materials, cursor, sizeof(uint));
					cursor += sizeof(uint);
					for (int i = 0; i < n_materials; i++) {
						uint64 uid = 0u;
						std::memcpy(&uid, cursor, sizeof(uint64));
						Resource* res = App->resources->CreateResource(Resource::Type::TEXTURE, uid);
						res->SetResourcePath((MATERIAL_L_FOLDER + std::to_string(uid) + ".dds").c_str());
						cursor += sizeof(uint64);
					}*/

					delete[] data;
				}
				aiReleaseImport(scene);
				return res->GetUID();
			}
			else {
				meta = App->dummy_file_system->GetUIDFromMeta((std::string(path) + ".meta").data());

				char* data = App->dummy_file_system->GetData((std::string(path) + ".meta").data());
				if (data != nullptr) {
					char* cursor = data + sizeof(uint64);

					uint n_meshes = 0u;
					memcpy(&n_meshes, cursor, sizeof(uint));
					cursor += sizeof(uint);
					for (int i = 0; i < n_meshes; i++) {
						uint64 uid = 0u;
						memcpy(&uid, cursor, sizeof(uint64));
						uid_meshes.push_back(uid);
						cursor += sizeof(uint64);
					}

					uint n_materials = 0u;
					std::memcpy(&n_materials, cursor, sizeof(uint));
					cursor += sizeof(uint);
					for (int i = 0; i < n_materials; i++) {
						uint64 uid = 0u;
						std::memcpy(&uid, cursor, sizeof(uint64));
						uid_materials.push_back(uid);
						cursor += sizeof(uint64);
					}

					delete[] data;
				}
			}
		}
		// -------------------------------------------------------------------

		App->object_manager->SetSelected(nullptr);
		uint ticks = SDL_GetTicks(); //timer
		PerfTimer timer;

		ResourceModel* model = nullptr;
		if (meta == 0u) {
			model = (ResourceModel*)App->resources->CreateResource(Resource::Type::MODEL);
		}
		else {
			model = (ResourceModel*)App->resources->CreateResource(Resource::Type::MODEL, meta);
			want_to_generate_meta = false;
		}
		
		model->SetFile(path);
		model->SetResourcePath((MODEL_L_FOLDER + std::to_string(model->GetUID()) + std::string(".whispModel")).c_str());

		bool equals = uid_meshes.size() == scene->mNumMeshes;
		want_to_generate_meta = !equals;
		if (!equals) uid_meshes.clear();

		for (int i = 0; i < scene->mNumMeshes; ++i) {
			if (equals)
				App->importer->mesh->Import(path, scene->mMeshes[i], model->GetUID(), uid_meshes[i]);
			else {
				uid_meshes.push_back(App->importer->mesh->Import(path, scene->mMeshes[i], model->GetUID()));
			}
		}

		equals = uid_materials.size() == scene->mNumMaterials;
		want_to_generate_meta = !equals;
		if (!equals) uid_materials.clear();

		for (int i = 0; i < scene->mNumMaterials; ++i) {
			if (equals)
				App->importer->material->Import(path, scene->mMaterials[i], uid_materials[i]);
			else {
				uid_materials.push_back(App->importer->material->Import(path, scene->mMaterials[i]));
			}
		}

		model->CalculateHierarchy(scene->mRootNode, scene, uid_meshes, uid_materials, nullptr);

		model->model.name = App->dummy_file_system->GetFileNameFromPath(path);
			 
		if (App->dummy_file_system->Exists(MODEL_L_FOLDER) == false) {
			App->dummy_file_system->CreateDir(MODEL_L_FOLDER);
		}

		if (want_to_generate_meta) {
			uint size = sizeof(uint) * 2 + sizeof(uint64_t) * uid_meshes.size() + sizeof(uint64_t) * uid_materials.size();
			char* data = new char[size];
			memset(data, 0, size);
			char* cursor = data;

			uint bytes = sizeof(uint);
			uint n_meshes = uid_meshes.size();
			memcpy(cursor, &n_meshes, bytes);

			cursor += bytes;
			bytes = sizeof(uint64_t) * n_meshes;
			memcpy(cursor, uid_meshes.data(), bytes);

			cursor += bytes;
			bytes = sizeof(uint);
			uint n_materials = uid_materials.size();
			memcpy(cursor, &n_materials, bytes);

			cursor += bytes;
			bytes = sizeof(uint64_t) * n_materials;
			memcpy(cursor, uid_materials.data(), bytes);

			App->dummy_file_system->GenerateMetaFile(path, model->GetUID(), data, size);
			delete[] data;
		}

		model->Save();

		aiReleaseImport(scene);
		LOG("Time to load FBX: %u", SDL_GetTicks() - ticks);
	}
	else
		LOG("Error loading scene: %s", scene == nullptr ? aiGetErrorString() : "The FBX has no meshes");

	return ret;
}
