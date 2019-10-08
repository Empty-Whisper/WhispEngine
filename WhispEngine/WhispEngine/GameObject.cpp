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

GameObject::GameObject(const int & n_vertex, float * vertex, const int & n_index, uint * index, float * normals)
	: n_vertex(n_vertex), n_index(n_index)
{
	InitColors();
	/*this->n_vertex = n_vertex;
	this->n_index = n_index;
	this->vertex = vertex;
	this->index = index;
	this->normals = normals;*/
	this->vertex = new float[n_vertex * 3];
	this->index = new uint[n_index * 3];
	if (normals != nullptr) {
		this->normals = new float[n_vertex * 3];
		memcpy(this->normals, normals, sizeof(float) * n_vertex * 3);
	}

	memcpy(this->vertex, vertex, sizeof(float) * n_vertex * 3);
	memcpy(this->index, index, sizeof(uint) * n_index);
	
	/*middle_point = new float[n_index];
	for (int i = 0; i < n_index; ++i) {
		middle_point[i] = (vertex[i] + vertex[i + 3] + vertex[i + 6]) / 3;
	}*/

	SetGLBuffers();
}

void GameObject::SetGLBuffers()
{
	//TEST NORMALS----------------------------------------------------------------
	/*middle_point = new float[n_index];
	for (int i = 0; i < n_index; ++i) {
		middle_point[i] = (vertex[i] + vertex[i + 3] + vertex[i + 6]) / 3;
	}*/
	// !TEST NORMALS--------------------------------------------------------

	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertex * 3, vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * n_index, index, GL_STATIC_DRAW);

	/*if (normals != nullptr) {
		glGenBuffers(1, &id_normals);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_vertex * 3, normals, GL_STATIC_DRAW);
	}*/
}

void GameObject::SetColors(const float * face_color, const float * wire_color)
{
	if (face_color != nullptr) {
		for (int i = 0; i < 3; ++i)
			color[i] = face_color[i];
	}
	if (wire_color != nullptr) {
		for (int i = 0; i < 3; ++i)
			this->wire_color[i] = wire_color[i];
	}
}

GameObject::~GameObject()
{
	delete[] vertex;
	delete[] index;
	delete[] normals;

	delete[] middle_point;
	
	delete[] color;
	delete[] wire_color;

	vertex = nullptr;
	index = nullptr;
}

void GameObject::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, n_index, GL_UNSIGNED_INT, NULL);

}

void GameObject::DrawNormals()
{
	if (normals != nullptr && middle_point != nullptr) {
		/*glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_POINTS, 0, n_vertex);*/
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINES);
		for (int i = 0; i < n_index * 3; i += 3) {
			glVertex3f(middle_point[i], middle_point[i + 1], middle_point[i + 2]);
			glVertex3f(middle_point[i] + normals[i], middle_point[i + 1] + normals[i + 1], middle_point[i + 2] + normals[i + 2]);
		}
		/*if (!vertex_normals) {
			for (int i = 0; i < n_vertex*3; i += 3) {
				glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
				glVertex3f(vertex[i] + normals[i], vertex[i + 1] + normals[i + 1], vertex[i + 2] + normals[i + 2]);
			}
		}
		else {
			for (int i = 0; i < n_vertex * 3; i += 3) {
				glVertex3f(middle_point[i], middle_point[i + 1], middle_point[i + 2]);
				glVertex3f(middle_point[i] + normals[i], middle_point[i + 1] + normals[i + 1], middle_point[i + 2] + normals[i + 2]);
			}
		}*/
		glEnd();
	}
}
