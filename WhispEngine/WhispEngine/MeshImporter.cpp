#include "MeshImporter.h"
#include "Application.h"
#include "Assimp/include/scene.h"
#include "MaterialImporter.h"
#include "ModuleImport.h"
#include "ComponentMaterial.h"
#include "ModuleObjectManager.h"
#include "Globals.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "mmgr/mmgr.h"

MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

uint64 MeshImporter::Import(const char * path, const aiMesh * mesh, const uint64 &parent_uid, const uint64 &force_uid)
{
	uint64 meta = force_uid;
	if (meta == 0u)
		meta = App->random->RandomGUID();

	ResourceMesh* ret = (ResourceMesh*)App->resources->CreateResource(Resource::MESH, meta);

	ret->SetFile(path);
	ret->SetResourcePath(std::string(MESH_L_FOLDER + std::to_string(ret->GetUID()) + ".whispMesh").data());
	ret->parent = parent_uid;

	/*length_name | num_vertex | num_index | num_face_normals | num_vertex_normals | num_tex_normals | ¿AABB?

	name | vertex | index | face_normals | vertex_normals | tex_normals*/

	ret->name.assign(mesh->mName.C_Str());

	uint header[5] = { mesh->mNumVertices, mesh->mNumFaces * 3, mesh->mNumFaces * 3 * 2, mesh->mNumVertices, mesh->mNumVertices };

	uint size =
		sizeof(header) +								//header
		sizeof(float) * mesh->mNumVertices * 3 + 		//vertex
		sizeof(uint) * mesh->mNumFaces * 9 + 			//index
		sizeof(float) * mesh->mNumFaces * 2 * 3 * 3 + 	//face_normals
		sizeof(float) * mesh->mNumVertices * 3 + 		//vertex_normals
		sizeof(float) * mesh->mNumVertices * 3;			//tex_normals

	char* data = new char[size];
	memset(data, 0, size);
	char* cursor = data;

	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh->mNumVertices * 3;
	memcpy(cursor, mesh->mVertices, bytes);

	cursor += bytes;
	if (mesh->HasFaces()) {
		uint* indices = (uint*)cursor;
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
				uint zero[3] = { 0u,0u,0u };
				memcpy(cursor, zero, sizeof(uint) * 3);
			}
			else
			{
				memcpy(cursor, mesh->mFaces[i].mIndices, sizeof(uint) * 3);
			}
			cursor += sizeof(uint) * 3;
		}

		bytes = sizeof(float) * mesh->mNumFaces * 2 * 3 * 3;
		float* face_normals = App->object_manager->CalculateFaceNormals((float*)mesh->mVertices, mesh->mNumFaces * 9 * 2, mesh->mNumFaces * 9, indices);
		memcpy(cursor, face_normals, bytes);
		delete[] face_normals;
	}
	else {
		cursor += sizeof(uint) * mesh->mNumFaces * 9 + sizeof(float) * mesh->mNumFaces * 2 * 3 * 3;
		LOG("Mesh has not faces");
	}

	if (mesh->HasNormals()) {
		cursor += bytes;
		bytes = sizeof(float) * mesh->mNumVertices * 3;
		memcpy(cursor, mesh->mNormals, bytes);
	}
	else {
		cursor += sizeof(float) * mesh->mNumVertices * 3;
		LOG("Mesh has not normals");
	}

	if (mesh->HasTextureCoords(0)) {
		cursor += bytes;
		bytes = sizeof(float) * mesh->mNumVertices * 3;
		memcpy(cursor, mesh->mTextureCoords[0], bytes);
	}
	else {
		LOG("Mesh has not texture coords");
	}

	if (App->file_system->Exists(MESH_L_FOLDER) == false)
		App->file_system->CreateDir(MESH_L_FOLDER);

	App->file_system->SaveData(data, size, ret->GetLibraryPath());
	delete[] data;

	return ret->GetUID();
}
