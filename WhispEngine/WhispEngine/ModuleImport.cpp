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
			obj->mesh.push_back(new Mesh());
			obj->mesh[i]->vertex.size = it->mNumVertices;
			obj->mesh[i]->vertex.data = new float[obj->mesh[i]->vertex.size * 3];
			memcpy(obj->mesh[i]->vertex.data, it->mVertices, sizeof(float) * obj->mesh[i]->vertex.size * 3);
			LOG("New mesh with %d mesh[i]->vertex.data", obj->mesh[i]->vertex.size);

			if (it->HasFaces())
			{
				obj->mesh[i]->index.size = it->mNumFaces * 3;
				obj->mesh[i]->index.data = new uint[obj->mesh[i]->index.size];

				for (uint j = 0; j < it->mNumFaces; ++j)
				{
					if (it->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&obj->mesh[i]->index.data[j * 3], it->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				LOG("New mesh with %i faces", obj->mesh[i]->index.size / 3);
			}

			if (it->HasNormals()) {

				float magnitude = 0.3f; // To multiply normalized vectors

				// Vertex Normals --------------------------------------------------
				obj->mesh[i]->vertex_normals.size = obj->mesh[i]->vertex.size;
				obj->mesh[i]->vertex_normals.data = new float[obj->mesh[i]->vertex_normals.size * 3];
				memcpy(obj->mesh[i]->vertex_normals.data, it->mNormals, sizeof(float) * obj->mesh[i]->vertex_normals.size * 3);
				for (int l = 0; l < obj->mesh[i]->vertex_normals.size * 3; ++l)
					obj->mesh[i]->vertex_normals.data[l] = obj->mesh[i]->vertex_normals.data[l] * magnitude + obj->mesh[i]->vertex.data[l];
				

				// Face Normals ----------------------------------------------------
				obj->mesh[i]->face_normals.size = obj->mesh[i]->index.size * 2;
				obj->mesh[i]->face_normals.data = new float[obj->mesh[i]->face_normals.size];

				for (int k = 0; k < obj->mesh[i]->index.size; k += 3) {
					vec3 p1(obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k]*3], obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k] * 3 + 1], obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k] * 3 + 2]);
					vec3 p2(obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k + 1] * 3], obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k + 1] * 3 + 1], obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k + 1] * 3 + 2]);
					vec3 p3(obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k + 2] * 3], obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k + 2] * 3 + 1], obj->mesh[i]->vertex.data[obj->mesh[i]->index.data[k + 2] * 3 + 2]);

					obj->mesh[i]->face_normals.data[k*2]		= (p1.x + p2.x + p3.x) / 3.f;
					obj->mesh[i]->face_normals.data[k*2 + 1]	= (p1.y + p2.y + p3.y) / 3.f;
					obj->mesh[i]->face_normals.data[k*2 + 2]	= (p1.z + p2.z + p3.z) / 3.f;

					vec3 v1 = p2 - p1;
					vec3 v2 = p3 - p1;
					
					vec3 v_norm = cross(v1, v2);
					v_norm = normalize(v_norm);

					obj->mesh[i]->face_normals.data[k * 2 + 3] = obj->mesh[i]->face_normals.data[k * 2] + v_norm.x * magnitude;
					obj->mesh[i]->face_normals.data[k * 2 + 4] = obj->mesh[i]->face_normals.data[k * 2 + 1] + v_norm.y * magnitude;
					obj->mesh[i]->face_normals.data[k * 2 + 5] = obj->mesh[i]->face_normals.data[k * 2 + 2] + v_norm.z * magnitude;
				}
			}

			obj->mesh[i]->SetGLBuffers();
			it = nullptr;
		}
		App->object_manager->AddObject(obj);

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene: %s", aiGetErrorString());

	return ret;
}
