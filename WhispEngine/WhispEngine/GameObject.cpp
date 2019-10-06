#include "GameObject.h"
#include "Application.h"
#include "Globals.h"

GameObject::GameObject(const int & n_vertex, const float * vertex, const int & n_index, const uint * index, const float * normals)
	: n_vertex(n_vertex), n_index(n_index)
{
	this->vertex = new float[n_vertex * 3];
	this->index = new uint[n_index * 3];
	if (normals != nullptr) {
		this->normals = new float[n_vertex * 3];
		memcpy(this->normals, normals, sizeof(float) * n_vertex * 3);
	}

	memcpy(this->vertex, vertex, sizeof(float) * n_vertex * 3);
	memcpy(this->index, index, sizeof(uint) * n_index);

	middle_point = new float[n_index * 3];
	for (int i = 0; i < n_index; ++i) {
		middle_point[i] = (vertex[i] + vertex[i + 3] + vertex[i + 6]) / 3;
	}

	SetGLBuffers();
}

void GameObject::SetGLBuffers()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertex * 3, vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * n_index, index, GL_STATIC_DRAW);

	if (normals != nullptr) {
		glGenBuffers(1, &id_normals);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertex * 3, normals, GL_STATIC_DRAW);
	}
}

GameObject::~GameObject()
{
	delete[] vertex;
	delete[] index;

	vertex = nullptr;
	index = nullptr;
}

void GameObject::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, n_index, GL_UNSIGNED_INT, NULL);

	if (normals != nullptr) {
		/*glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_POINTS, 0, n_vertex);*/
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		for (int i = 0; i < n_vertex; i+=3) {
			glVertex3f(middle_point[i], middle_point[i+1], middle_point[i+2]);
			glVertex3f(middle_point[i] + normals[i], middle_point[i+1] + normals[i+1], middle_point[i + 2] + normals[i+2]);
		}
		glEnd();
	}
}
