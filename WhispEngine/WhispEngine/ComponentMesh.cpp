#include "ComponentMesh.h"
#include "Application.h"
#include "Globals.h"

ComponentMesh::ComponentMesh(GameObject *parent) : Component(parent, ComponentType::MESH)
{
	material = (ComponentMaterial*)parent->GetComponent(ComponentType::MATERIAL);
}

void ComponentMesh::Update()
{
	glColor3f(1.f, 1.f, 1.f);
	glEnableClientState(GL_VERTEX_ARRAY);
	if (App->renderer3D->fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		Draw();
	}
	if (App->renderer3D->wireframe) {
		if (material != nullptr) {
			glColor4fv(material->GetWireColor());
		}
		else {
			glColor3i(0, 0, 0);
		}
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.f, 1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		DrawWireFrame();
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
	DrawNormals();
	glColor3f(0.f, 0.f, 0.f);

	glDisableClientState(GL_VERTEX_ARRAY);
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
}

void ComponentMesh::Draw()
{
	glColor3f(1.f, 1.f, 1.f);

	if (material != nullptr) {
		if (material->HasTexture()) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindTexture(GL_TEXTURE_2D, material->GetIDTexture());
		}
		else {
			glColor3fv(material->GetFaceColor());
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (mesh->tex_coords.data != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->tex_coords.id);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}
	if (mesh->vertex_normals.data != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_normals.id);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index.id);
	glDrawElements(GL_TRIANGLES, mesh->index.size, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ComponentMesh::DrawWireFrame() {
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index.id);
	glDrawElements(GL_TRIANGLES, mesh->index.size, GL_UNSIGNED_INT, NULL);
}

void ComponentMesh::DrawNormals()
{

	if (normals_state == Normals::FACE) {
		if (mesh->face_normals.data != nullptr) {
			glColor3f(0.f, 1.f, 0.f);
			glLineWidth(3.f);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->face_normals.id);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawArrays(GL_LINES, 0, mesh->face_normals.size);
			glLineWidth(1.f);
		}
	}
	if (normals_state == Normals::VERTEX) {
		glColor3f(0.f, 0.f, 1.f);

		if (mesh->vertex_normals.data != nullptr) {
			glBegin(GL_LINES);
			for (int j = 0; j < mesh->vertex.size * 2; j += 3) {
				glVertex3f(mesh->vertex.data[j], mesh->vertex.data[j + 1], mesh->vertex.data[j + 2]);
				glVertex3f(mesh->vertex_normals.data[j], mesh->vertex_normals.data[j + 1], mesh->vertex_normals.data[j + 2]);
			}
			glEnd();
		}
	}
}

void ComponentMesh::SetMaterial(ComponentMaterial  * mat)
{
	material = mat;
}

void ComponentMesh::OnInspector()
{
	if (ImGui::CollapsingHeader("Mesh")) {
		ImGui::Checkbox("Active", &is_active);

		/*ImGui::Checkbox("UVs", &view_uv);
		ImGui::Checkbox("Face Normals", &view_face_normals);
		ImGui::Checkbox("Vertex Normals", &view_vertex_normals);*/
	}
}


Mesh_info::~Mesh_info()
{
	delete[] vertex.data;
	delete[] index.data;
	delete[] face_normals.data;
	if (vertex_normals.data != nullptr)
		delete[] vertex_normals.data;
	if (tex_coords.data != nullptr)
		delete[] tex_coords.data;

	glDeleteBuffers(1, &vertex.id);
	glDeleteBuffers(1, &index.id);
	glDeleteBuffers(1, &face_normals.id);
	glDeleteBuffers(1, &tex_coords.id);
}

void Mesh_info::SetGLBuffers()
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

	if (vertex_normals.data != nullptr) { // TODO Correct draw
		glGenBuffers(1, &vertex_normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_normals.size, vertex_normals.data, GL_STATIC_DRAW);
	}
}
