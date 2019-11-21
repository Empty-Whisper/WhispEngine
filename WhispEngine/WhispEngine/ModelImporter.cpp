#include "ModelImporter.h"
#include "Application.h"
#include "ComponentTransform.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

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
				return App->dummy_file_system->GetMeta((std::string(path) + ".meta").data());
			}
		}
		// -------------------------------------------------------------------

		App->object_manager->SetSelected(nullptr);
		uint ticks = SDL_GetTicks(); //timer
		PerfTimer timer;

		ResourceModel* model = (ResourceModel*)App->resources->CreateResource(Resource::Type::MODEL);
		App->dummy_file_system->GenerateMetaFile(path, model->GetUID());
		model->SetFile(path);
		model->SetResourcePath((MODEL_L_FOLDER + std::to_string(model->GetUID()) + std::string(".whispModel")).c_str());

		std::vector<uint64> uid_meshes, uid_materials;

		for (int i = 0; i < scene->mNumMeshes; ++i) {
			uid_meshes.push_back(App->importer->mesh->Import(path, scene->mMeshes[i], model->GetUID()));
		}

		for (int i = 0; i < scene->mNumMaterials; ++i) {
			uid_materials.push_back(App->importer->material->Import(path, scene->mMaterials[i]));
		}

		model->CalculateHierarchy(scene->mRootNode, scene, uid_meshes, uid_materials, nullptr);

		model->GetModel().name = App->dummy_file_system->GetFileNameFromPath(path);
			 
		if (App->dummy_file_system->Exists(MODEL_L_FOLDER) == false) {
			App->dummy_file_system->CreateDir(MODEL_L_FOLDER);
		}

		model->Save();

		aiReleaseImport(scene);
		LOG("Time to load FBX: %u", SDL_GetTicks() - ticks);
	}
	else
		LOG("Error loading scene: %s", scene == nullptr ? aiGetErrorString() : "The FBX has no meshes");

	return ret;
}
