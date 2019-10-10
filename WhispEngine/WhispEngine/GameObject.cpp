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
	color[1] = 1.f;
	color[2] = 1.f;
	wire_color = new float[3];
	wire_color[0] = 0.f;
	wire_color[1] = 0.f;
	wire_color[2] = 0.f;
}

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
	glColor3f(1.f, 1.f, 1.f);
	for (int i = 0; i < mesh.size(); ++i) {
		if (App->object_manager->GetTexture() != nullptr) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindTexture(GL_TEXTURE_2D, App->object_manager->GetTexture()->id);
		}
		else {
			glColor3fv(color);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->vertex.id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (mesh[i]->tex_coords.data != nullptr) {
			glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->tex_coords.id);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}
		if (mesh[i]->vertex_normals.data != nullptr) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->vertex_normals.id);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh[i]->index.id);
		glDrawElements(GL_TRIANGLES, mesh[i]->index.size, GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GameObject::DrawWireFrame() {
	glColor3fv(wire_color);
	for (int i = 0; i < mesh.size(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->vertex.id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh[i]->index.id);
		glDrawElements(GL_TRIANGLES, mesh[i]->index.size, GL_UNSIGNED_INT, NULL);
	}
}

void GameObject::DrawNormals()
{

	for (int i = 0; i < mesh.size(); ++i) {
		if (normals_state == Normals::FACE) {
			if (mesh[i]->face_normals.data != nullptr) {
				glColor3f(0.f, 1.f, 0.f);
				glLineWidth(3.f);
				glBindBuffer(GL_ARRAY_BUFFER, mesh[i]->face_normals.id);
				glVertexPointer(3, GL_FLOAT, 0, NULL);
				glDrawArrays(GL_LINES, 0, mesh[i]->face_normals.size);
				glLineWidth(1.f);
			}
		}
		if (normals_state == Normals::VERTEX) {
			glColor3f(0.f, 0.f, 1.f);

			if (mesh[i]->vertex_normals.data != nullptr) {
				glBegin(GL_LINES);
				for (int j = 0; j < mesh[i]->vertex.size * 2; j += 3) {
					glVertex3f(mesh[i]->vertex.data[j], mesh[i]->vertex.data[j + 1], mesh[i]->vertex.data[j + 2]);
					glVertex3f(mesh[i]->vertex_normals.data[j], mesh[i]->vertex_normals.data[j + 1], mesh[i]->vertex_normals.data[j + 2]);
				}
				glEnd();
			}
		}
	}
}


Mesh::~Mesh()
{
	delete[] vertex.data;
	delete[] index.data;
	delete[] face_normals.data;
	if (vertex_normals.data != nullptr)
		delete[] vertex_normals.data;
	delete[] tex_coords.data;

	glDeleteBuffers(1, &vertex.id);
	glDeleteBuffers(1, &index.id);
	glDeleteBuffers(1, &face_normals.id);
	glDeleteBuffers(1, &tex_coords.id);
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

	if (tex_coords.data != nullptr) {
		glGenBuffers(1, &tex_coords.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tex_coords.id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * tex_coords.size * 3, tex_coords.data, GL_STATIC_DRAW);
	}

	if (vertex_normals.data != nullptr) { // TODO Set Vertex mesh with a buffer and draw with it
		glGenBuffers(1, &vertex_normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_normals.size, vertex_normals.data, GL_STATIC_DRAW);
	}
}
