#include "GameObject.h"
#include "Application.h"
#include "Globals.h"

GameObject::GameObject()
{
	InitColors();
}

void GameObject::InitColors()
{
	color = new float[3];
	color[0] = 1.f;
	wire_color = new float[3];
	wire_color[0] = 0.f;
	wire_color[1] = 0.f;
	wire_color[2] = 0.f;
}

//GameObject::GameObject(const int & n_vertex, float * vertex, const int & n_index, uint * index, float * normals)
//	: n_vertex(n_vertex), n_index(n_index)
//{
//	InitColors();
//	this->vertex = new float[n_vertex * 3];
//	this->index = new uint[n_index * 3];
//	if (normals != nullptr) {
//		this->normals = new float[n_vertex * 3];
//		memcpy(this->normals, normals, sizeof(float) * n_vertex * 3);
//	}
//
//	memcpy(this->vertex, vertex, sizeof(float) * n_vertex * 3);
//	memcpy(this->index, index, sizeof(uint) * n_index);
//	
//	/*middle_point = new float[n_index];
//	for (int i = 0; i < n_index; ++i) {
//		middle_point[i] = (vertex[i] + vertex[i + 3] + vertex[i + 6]) / 3;
//	}*/
//
//	SetGLBuffers();
//}

void GameObject::SetColors(const float * face_color, const float * wire_c)
{
	if (face_color != nullptr) {
		for (int i = 0; i < 3; ++i)
			color[i] = face_color[i];
	}
	if (wire_c != nullptr) {
		for (int i = 0; i < 3; ++i)
			this->wire_color[i] = wire_c[i];
	}
}

GameObject::~GameObject()
{
	for (int i = 0; i < mesh.size(); ++i) {
		delete mesh[i];
	}
	
	delete[] color;
	delete[] wire_color;
}

void GameObject::Draw()
{
	for (int i = 0; i < mesh.size(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->vertex.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh[i]->index.id);

		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, mesh[i]->index.size, GL_UNSIGNED_INT, NULL);
	}
}

void GameObject::DrawNormals()
{
	for (int i = 0; i < mesh.size(); ++i) {
		if (mesh[i]->face_normals.data != nullptr) {
			glColor3f(0.f, 1.f, 0.f);
			glLineWidth(3.f);
			glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->face_normals.id);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawArrays(GL_LINES, 0, mesh[i]->face_normals.size);
			glLineWidth(1.f);

			glColor3f(0.f, 0.f, 1.f);
			/*glBindBuffer(GL_ARRAY_BUFFER, vertex_normals.id);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawArrays(GL_POINTS, 0, vertex_normals.size);*/

			glBegin(GL_LINES);
			for (int j = 0; j < mesh[i]->index.size * 2; j += 3) {
				glVertex3f(mesh[i]->vertex.data[j], mesh[i]->vertex.data[j + 1], mesh[i]->vertex.data[j + 2]);
				glVertex3f(mesh[i]->vertex_normals.data[j], mesh[i]->vertex_normals.data[j + 1], mesh[i]->vertex_normals.data[j + 2]);
			}
			glEnd();
		}
	}
}

Mesh::~Mesh()
{
	delete[] vertex.data;
	delete[] index.data;
	delete[] face_normals.data;

	glDeleteBuffers(1, &vertex.id);
	glDeleteBuffers(1, &index.id);
	glDeleteBuffers(1, &face_normals.id);
}

void Mesh::SetGLBuffers()
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
	/*if (mesh[i]->vertex_normals.data != nullptr) { // TODO Set Vertex mesh with a buffer and draw with it
		glGenBuffers(1, &mesh[i]->vertex_normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->vertex_normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh[i]->vertex_normals.size, mesh[i]->vertex_normals.data, GL_STATIC_DRAW);
	}*/
}
