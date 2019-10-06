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
		uint n_v = 0;
		uint n_i = 0;
		float* v = nullptr;
		uint* a_i = nullptr;

		float* normals = nullptr;

		aiMesh* it = nullptr;
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			it = scene->mMeshes[i];
			n_v = it->mNumVertices;
			v = new float[n_v * 3];
			memcpy(v, it->mVertices, sizeof(float) * n_v * 3);
			LOG("New mesh with %d vertex", n_v);

			if (it->HasFaces())
			{
				n_i = it->mNumFaces * 3;
				a_i = new uint[n_i];

				for (uint j = 0; j < it->mNumFaces; ++j)
				{
					if (it->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&a_i[j * 3], it->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				LOG("New mesh with %i faces", n_i / 3);
			}

			if (it->HasNormals()) {
				normals = new float[n_v * 3];
				memcpy(normals, it->mNormals, sizeof(float) * n_v * 3);

				for (int i = 0; i < n_v * 3; i += 3) {
					/*normals[i] *= 5;
					normals[i + 1] *= 5;
					normals[i + 2] *= 5;*/
					LOG("n(%i): %.2f %.2f %.2f", i / 3, normals[i], normals[i + 1], normals[i + 2]);

				}
			}

			App->object_manager->AddObject(new GameObject(n_v,v,n_i,a_i,normals));

			/*delete[] v;
			v = nullptr;
			delete[] a_i;
			a_i = nullptr;
			delete[] normals;*/

			it = nullptr;
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene: %s", aiGetErrorString());

	return ret;
}
