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
	
	//App->file_system->Save(std::string(MESH_L_FOLDER + std::to_string(uid) + ".whispMesh").data(), data, size);
	App->dummy_file_system->SaveData(data, size, std::string(MESH_L_FOLDER + std::to_string(uid) + ".whispMesh").data());
	delete[] data;

	return true;
}

bool MeshImporter::Load(const uint64_t & uid, Mesh_info * mesh)
{
	std::string path(MESH_L_FOLDER + std::to_string(uid) + ".whispMesh");

	char * data = App->dummy_file_system->GetData(path.c_str());
	
	if (data == nullptr) {
		return false;
	}
	char* cursor = data;


	uint header[5];
	uint bytes = sizeof(header);
	memcpy(header, cursor, bytes);

	uint num_vertices		= header[0];
	uint num_index			= header[1];
	uint num_face_normals	= header[2];
	uint num_vertex_normals = header[3];
	uint num_tex_coords		= header[4];

	cursor += bytes;
	mesh->vertex.size = num_vertices;
	bytes = sizeof(float) * mesh->vertex.size * 3;
	mesh->vertex.data = new float[mesh->vertex.size * 3];
	memcpy(mesh->vertex.data, cursor, bytes);

	cursor += bytes;

	mesh->index.size = num_index * 3;
	bytes = num_index * sizeof(uint);
	mesh->index.data = new uint[mesh->index.size];
	memcpy(mesh->index.data, cursor, bytes);

	cursor += bytes;
	mesh->face_normals.size = num_face_normals * 3;
	bytes = mesh->face_normals.size * sizeof(float);
	mesh->face_normals.data = new float[mesh->face_normals.size];
	memcpy(mesh->face_normals.data, cursor, bytes);

	cursor += bytes;
	mesh->vertex_normals.size = num_vertex_normals * 3;
	bytes = mesh->vertex_normals.size * sizeof(float);
	mesh->vertex_normals.data = new float[mesh->vertex_normals.size];
	memcpy(mesh->vertex_normals.data, cursor, bytes);

	cursor += bytes;
	mesh->tex_coords.size = num_tex_coords * 3;
	bytes = mesh->tex_coords.size * sizeof(float);
	mesh->tex_coords.data = new float[mesh->tex_coords.size];
	memcpy(mesh->tex_coords.data, cursor, bytes);

	mesh->SetGLBuffers();

	delete[] data;
	return true;
}

