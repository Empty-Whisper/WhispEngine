#include "ResourceMesh.h"
#include "Application.h"

#include "glew-2.1.0/include/GL/glew.h"

ResourceMesh::ResourceMesh(const uint64& uid) : Resource(uid, Resource::Type::MESH)
{
}


ResourceMesh::~ResourceMesh()
{
	if (references > 0u)
		FreeMemory();
	references = 0u;
}

bool ResourceMesh::LoadInMemory()
{
	char * data = App->dummy_file_system->GetData(resource_path.c_str());

	if (data == nullptr) {
		LOG("Failed, mesh with uid %" PRIu64 " not found", uid);
		return false;
	}
	char* cursor = data;

	uint header[5];
	uint bytes = sizeof(header);
	memcpy(header, cursor, bytes);

	uint num_vertices = header[0];
	uint num_index = header[1];
	uint num_face_normals = header[2];
	uint num_vertex_normals = header[3];
	uint num_tex_coords = header[4];

	cursor += bytes;
	vertex.size = num_vertices;
	bytes = sizeof(float) * vertex.size * 3;
	vertex.data = new float[vertex.size * 3];
	memcpy(vertex.data, cursor, bytes);

	cursor += bytes;

	index.size = num_index * 3;
	bytes = num_index * sizeof(uint);
	index.data = new uint[index.size];
	memcpy(index.data, cursor, bytes);

	cursor += bytes;
	face_normals.size = num_face_normals * 3;
	bytes = face_normals.size * sizeof(float);
	face_normals.data = new float[face_normals.size];
	memcpy(face_normals.data, cursor, bytes);

	cursor += bytes;
	vertex_normals.size = num_vertex_normals * 3;
	bytes = vertex_normals.size * sizeof(float);
	vertex_normals.data = new float[vertex_normals.size];
	memcpy(vertex_normals.data, cursor, bytes);

	cursor += bytes;
	tex_coords.size = num_tex_coords;
	bytes = tex_coords.size * sizeof(float) * 3;
	tex_coords.data = new float[tex_coords.size * 3];
	memcpy(tex_coords.data, cursor, bytes);

	SetGLBuffers();

	delete[] data;
	return true;
}

bool ResourceMesh::FreeMemory()
{
	if (vertex.data != nullptr)
		delete[] vertex.data;
	if (index.data != nullptr)
		delete[] index.data;
	if (face_normals.data != nullptr)
		delete[] face_normals.data;
	if (vertex_normals.data != nullptr)
		delete[] vertex_normals.data;
	if (tex_coords.data != nullptr)
		delete[] tex_coords.data;

	glDeleteBuffers(1, &vertex.id);
	glDeleteBuffers(1, &index.id);
	glDeleteBuffers(1, &face_normals.id);
	glDeleteBuffers(1, &tex_coords.id);

	return true;
}

void ResourceMesh::SetGLBuffers()
{
	glGenBuffers(1, &vertex.id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex.id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size * 3, vertex.data, GL_STATIC_DRAW);

	glGenBuffers(1, &index.id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index.size, index.data, GL_STATIC_DRAW);

	if (face_normals.data != nullptr) {
		glGenBuffers(1, &face_normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, face_normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * face_normals.size, face_normals.data, GL_STATIC_DRAW);
	}

	if (tex_coords.data != nullptr) {
		glGenBuffers(1, &tex_coords.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tex_coords.id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * tex_coords.size * 3, tex_coords.data, GL_STATIC_DRAW);
	}

	if (vertex_normals.data != nullptr) {
		glGenBuffers(1, &vertex_normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_normals.size, vertex_normals.data, GL_STATIC_DRAW);
	}
}
