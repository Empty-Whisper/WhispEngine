#include "ModuleImport.h"

#include "Application.h"
#include "ModuleObjectManager.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

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

	return true;
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

bool ModuleImport::ImportFile(const char * path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		/*uint n_v = 0;
		uint n_i = 0;
		float* v = nullptr;
		uint* a_i = nullptr;

		float* normals = nullptr;*/

		GameObject *obj = new GameObject();

		aiMesh* it = nullptr;
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			it = scene->mMeshes[i];
			obj->n_vertex = it->mNumVertices;
			obj->vertex = new float[obj->n_vertex * 3];
			memcpy(obj->vertex, it->mVertices, sizeof(float) * obj->n_vertex * 3);
			LOG("New mesh with %d vertex", obj->n_vertex);

			if (it->HasFaces())
			{
				obj->n_index = it->mNumFaces * 3;
				obj->index = new uint[obj->n_index];

				for (uint j = 0; j < it->mNumFaces; ++j)
				{
					if (it->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&obj->index[j * 3], it->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				LOG("New mesh with %i faces", obj->n_index / 3);
			}

			if (it->HasNormals()) {
				obj->normals = new float[obj->n_vertex * 3];
				memcpy(obj->normals, it->mNormals, sizeof(float) * obj->n_vertex * 3);

				/*for (int i = 0; i < n_v * 3; i += 3) {
					LOG("n(%i): %.2f %.2f %.2f", i / 3, normals[i], normals[i + 1], normals[i + 2]);
				}*/
			}

			obj->SetGLBuffers();
			App->object_manager->AddObject(obj);

			it = nullptr;
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene: %s", aiGetErrorString());

	return ret;
}
