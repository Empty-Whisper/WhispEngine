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
	uint header[5] = { mesh->mNumVertices, mesh->mNumFaces, mesh->mNumFaces * 2, mesh->mNumVertices, mesh->mNumVertices };

	uint size =
		sizeof(header) +								//header
		sizeof(float) * mesh->mNumVertices * 3 + 		//vertex
		sizeof(uint) * mesh->mNumFaces * 9; //+ 			//index
		//sizeof(float) * mesh->mNumFaces * 2 * 3 * 3 + 	//face_normals
		//sizeof(float) * mesh->mNumVertices * 3 + 		//vertex_normals
		//sizeof(float) * mesh->mNumVertices * 3;			//tex_normals

	char* data = new char[size];
	memset(data, 0, size);
	char* cursor = data;

	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh->mNumVertices * 3;
	memcpy(cursor, mesh->mVertices, bytes);
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		LOG("Vertex %i: %.3f, %.3f, %.3f", i, mesh->mVertices[i * 3], mesh->mVertices[i * 3 + 1], mesh->mVertices[i * 3 + 2])
	}

	cursor += bytes;
	if (mesh->HasFaces()) {

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
				uint* num = (uint*)cursor;
				LOG("Face %i: %u, %u, %u", i, num[0], num[1], num[2]);
			}
			cursor += sizeof(uint) * 3;
		}
		//bytes = sizeof(uint) * mesh->mNumFaces * 9;

		//cursor += bytes;
		//bytes = sizeof(float) * mesh->mNumFaces * 2 * 3 * 3;
		//memcpy(cursor, face_normals, bytes); TODO: Calculate face normals
		//memset(cursor, NULL, bytes);
	}
	else LOG("Mesh has not faces");

	/*if (mesh->HasNormals()) {
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
	else LOG("Mesh has not texture coords");*/
	
	//App->dummy_file_system->SaveData(data, std::string(MESH_L_FOLDER + std::to_string(uid) + ".whispMesh").data(), size);
	
	App->file_system->Save(std::string(MESH_L_FOLDER + std::to_string(uid) + ".whispMesh").data(), data, size);
	delete[] data;

	return true;
}

bool MeshImporter::Load(const uint64_t & uid, Mesh_info * mesh)
{
	std::string path(MESH_L_FOLDER + std::to_string(uid) + ".whispMesh");

	char * data = App->dummy_file_system->GetData(path.c_str());
	//App->file_system.
	if (data == nullptr) {
		return false;
	}
	char* cursor = data;


	uint header[5];
	uint bytes = sizeof(header);
	memcpy(header, cursor, bytes);

	uint num_vertices = header[0];
	uint num_index = header[1];

	cursor += bytes;
	mesh->vertex.size = num_vertices;
	bytes = sizeof(float) * mesh->vertex.size * 3;
	mesh->vertex.data = new float[mesh->vertex.size * 3];
	memcpy(mesh->vertex.data, cursor, bytes);

	cursor += bytes;

	mesh->index.size = num_index * 3 * 3;
	bytes = mesh->index.size * sizeof(uint);
	mesh->index.data = new uint[mesh->index.size];

	uint * num = (uint*)cursor;
	for (int i = 0; i < num_index; i++) {
		LOG("%u", num[i]);
	}
	memcpy(mesh->index.data, cursor, bytes);

	mesh->SetGLBuffers();

	delete[] data;
	return true;
}

