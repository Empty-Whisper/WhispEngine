#include "MeshImporter.h"
#include "Application.h"
#include "Assimp/include/scene.h"
#include "Globals.h"

MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Import(const uint64_t &uid, const aiMesh* mesh)
{
	/*length_name | num_vertex | num_index | num_face_normals | num_vertex_normals | num_tex_normals | ¿AABB?

	name | vertex | index | face_normals | vertex_normals | tex_normals*/

	std::string name(mesh->mName.C_Str());
	uint header[] = { name.length(), mesh->mNumVertices, mesh->mNumFaces * 3, mesh->mNumFaces * 2, mesh->mNumVertices, mesh->mNumVertices };

	uint size = 
		sizeof(header) +								//header
		name.length() +									//name
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
	bytes = name.length();
	memcpy(cursor, name.c_str(), bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh->mNumVertices * 3;
	memcpy(cursor, (float*)mesh->mVertices, bytes);

	if (mesh->HasFaces()) {
		cursor += bytes;

		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
				cursor[i * 3]	  = 0;
				cursor[i * 3 + 1] = 0;
				cursor[i * 3 + 2] = 0;
			}
			else
			{
				memcpy(&cursor[i*3], mesh->mFaces[i].mIndices, sizeof(uint) * 3);
			}
		}
		bytes = sizeof(uint) * mesh->mNumFaces * 9;

		cursor += bytes;
		bytes = sizeof(float) * mesh->mNumFaces * 2 * 3 * 3;
		//memcpy(cursor, face_normals, bytes); TODO: Calculate face normals
		memset(cursor, NULL, bytes);
	}
	else LOG("Mesh has not faces");

	if (mesh->HasNormals()) {
		cursor += bytes;
		bytes = sizeof(float) * mesh->mNumVertices * 3;
		memcpy(cursor, (float*)mesh->mNormals, bytes);
	}
	else LOG("Mesh has not normals");

	if (mesh->HasTextureCoords(0)) {
		cursor += bytes;
		bytes = sizeof(float) * mesh->mNumVertices * 3;
		memcpy(cursor, (float*)mesh->mTextureCoords[0], bytes);
	}
	else LOG("Mesh has not texture coords");
	
	App->file_system->SaveData(data, std::string(MESH_LFOLDER + std::to_string(uid) + ".whispMesh").data(), size);
	delete[] data;

	return true;
}

bool MeshImporter::Load(const uint64_t & uid, Mesh_info * mesh)
{
	std::string path(MESH_LFOLDER + std::to_string(uid) + ".whispMesh");

	char * data = App->file_system->GetData(path.c_str());
	if (data == nullptr) {
		return false;
	}
	char* cursor = data;


	uint header[6];
	uint bytes = sizeof(header);
	memcpy(header, cursor, bytes);

	mesh->vertex.size = header[1] / sizeof(float);
	mesh->index.size = header[2] / sizeof(uint);

	cursor += bytes;
	bytes = header[0];
	std::string name(cursor); //TODO: set name in model file, the mesh has not any relation with object's name
	//mesh->component->object->SetName(name.c_str());

	cursor += bytes;
	bytes = mesh->vertex.size * 3;
	mesh->vertex.data = new float[mesh->vertex.size];
	memcpy(mesh->vertex.data, cursor, bytes);

	cursor += bytes;
	bytes = mesh->index.size * 3;
	mesh->index.data = new uint[mesh->index.size];
	memcpy(mesh->index.data, cursor, bytes);

	mesh->SetGLBuffers();

	delete[] data;
	return true;
}

