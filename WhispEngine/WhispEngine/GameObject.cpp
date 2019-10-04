#include "GameObject.h"
#include "Application.h"
#include "Globals.h"

GameObject::GameObject(const int & n_vertex, const float * vertex, const int & n_index, const uint * index)
	: n_vertex(n_vertex), n_index(n_index)
{
	this->vertex = new float[n_vertex * 3];
	this->index = new uint[n_index * 3];

	memcpy(this->vertex, vertex, sizeof(float) * n_vertex * 3);
	memcpy(this->index, index, sizeof(uint) * n_index * 3);


	SetGLBuffers();
}

void GameObject::SetGLBuffers()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertex * 3, vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * n_index * 3, index, GL_STATIC_DRAW);
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
	glDrawElements(GL_TRIANGLES, n_index * 3, GL_UNSIGNED_SHORT, NULL);
}
