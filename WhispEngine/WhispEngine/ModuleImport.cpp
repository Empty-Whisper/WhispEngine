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
		GameObject *obj = new GameObject();

		aiMesh* it = nullptr;
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			it = scene->mMeshes[i];
			obj->vertex.size = it->mNumVertices;
			obj->vertex.data = new float[obj->vertex.size * 3];
			memcpy(obj->vertex.data, it->mVertices, sizeof(float) * obj->vertex.size * 3);
			LOG("New mesh with %d vertex.data", obj->vertex.size);

			if (it->HasFaces())
			{
				obj->index.size = it->mNumFaces * 3;
				obj->index.data = new uint[obj->index.size];

				for (uint j = 0; j < it->mNumFaces; ++j)
				{
					if (it->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&obj->index.data[j * 3], it->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				LOG("New mesh with %i faces", obj->index.size / 3);
			}

			if (it->HasNormals()) {

				obj->normals = new float[obj->index.size];
				obj->middle_point = new float[obj->index.size];
				for (int k = 0; k < obj->index.size; k += 3) {
					vec3 p1(obj->vertex.data[obj->index.data[k]*3], obj->vertex.data[obj->index.data[k] * 3 + 1], obj->vertex.data[obj->index.data[k] * 3 + 2]);
					vec3 p2(obj->vertex.data[obj->index.data[k + 1] * 3], obj->vertex.data[obj->index.data[k + 1] * 3 + 1], obj->vertex.data[obj->index.data[k + 1] * 3 + 2]);
					vec3 p3(obj->vertex.data[obj->index.data[k + 2] * 3], obj->vertex.data[obj->index.data[k + 2] * 3 + 1], obj->vertex.data[obj->index.data[k + 2] * 3 + 2]);

					obj->middle_point[k]		= (p1.x + p2.x + p3.x) / 3.f;
					obj->middle_point[k + 1]	= (p1.y + p2.y + p3.y) / 3.f;
					obj->middle_point[k + 2]	= (p1.z + p2.z + p3.z) / 3.f;

					vec3 v1 = p2 - p1;
					vec3 v2 = p3 - p1;
					
					vec3 v_norm = cross(v1, v2);
					v_norm = normalize(v_norm);

					float magnitude = 0.3f;
					obj->normals[k] =		v_norm.x * magnitude;
					obj->normals[k + 1] =   v_norm.y * magnitude;
					obj->normals[k + 2] =   v_norm.z * magnitude;
				}
				
				/*memcpy(obj->normals, it->mNormals, sizeof(float) * obj->n_vertex * 3);*/

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
