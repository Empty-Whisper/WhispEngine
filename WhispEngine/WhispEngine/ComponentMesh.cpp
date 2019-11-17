#include "ComponentMesh.h"
#include "Application.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/MathFunc.h"


ComponentMesh::ComponentMesh(GameObject *parent) : Component(parent, ComponentType::MESH)
{
	material = (ComponentMaterial*)parent->CreateComponent(ComponentType::MATERIAL);
}

void ComponentMesh::Update()
{
	if (mesh == nullptr)
		return;

	glColor3f(1.f, 1.f, 1.f);
	glEnableClientState(GL_VERTEX_ARRAY);

	if (App->renderer3D->wireframe) {
		if (material != nullptr) {
			glColor4fv(material->GetWireColor());
		}
		else {
			glColor3i(0, 0, 0);
		}
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.f, 1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		DrawWireFrame();
	}
	if (object->GetSelect() != ObjectSelected::NONE)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		DrawOutline();
	}
	if(App->renderer3D->fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (object->is_inside_frustum)
			Draw();
	}
	DrawNormals();
	glColor3f(0.f, 0.f, 0.f);

	glDisableClientState(GL_VERTEX_ARRAY);
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
	if (object->GetComponent(ComponentType::MATERIAL) != nullptr) {
		if (material != nullptr) {
			object->components.erase(std::find(object->components.begin(), object->components.end(), material));
			delete material;
			material = nullptr;
		}
	}
}

void ComponentMesh::Draw()
{
	
	glColor3f(1.f, 1.f, 1.f);

		if (mesh->tex_coords.data != nullptr) {
			if (material->IsActive()) {
				if (material->HasTexture()) {
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindTexture(GL_TEXTURE_2D, material->GetIDTexture());
				}
				else {
					glColor3fv(material->GetFaceColor());
				}
			}
		}

		if (mesh->vertex.data != nullptr) {
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
		}

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
	
	
}

void ComponentMesh::DrawWireFrame() {
	glDisable(GL_LIGHTING);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index.id);
	glDrawElements(GL_TRIANGLES, mesh->index.size, GL_UNSIGNED_INT, NULL);
	glEnable(GL_LIGHTING);
}

void ComponentMesh::DrawOutline()
{
	if (glIsEnabled(GL_STENCIL_TEST))
	{

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		// Render the mesh into the stencil buffer.

		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		Draw();

		// Render the thick wireframe version.

		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


		if (object->GetSelect() == ObjectSelected::SELECTED)
		{
			glColor3f(1.f, 0.7f, 0.f);
			glLineWidth(7);
		}
		else if (object->GetSelect() == ObjectSelected::CHILD_FROM_PARENT_SELECTED)
		{
			glColor3f(0.f, 1.f, 1.f);
			glLineWidth(4);
		}		
		glPolygonMode(GL_FRONT, GL_LINE);

		DrawWireFrame();


		/*--------------------------------------*/
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisableClientState(GL_VERTEX_ARRAY);
		glLineWidth(1);
	}

}

void ComponentMesh::DrawNormals()
{

	if (view_face_normals) {
		if (mesh->face_normals.data != nullptr) {
			glColor3f(0.f, 1.f, 0.f);
			glLineWidth(3.f);
			glBegin(GL_LINES);
			for (int i = 0; i < mesh->face_normals.size; i += 6) {
				glVertex3f(mesh->face_normals.data[i], mesh->face_normals.data[i + 1], mesh->face_normals.data[i + 2]);
				glVertex3f(mesh->face_normals.data[i + 3], mesh->face_normals.data[i + 4], mesh->face_normals.data[i + 5]);
			}
			glEnd();
			glLineWidth(1.f);
		}
	}
	if (view_vertex_normals) {
		glColor3f(0.f, 0.f, 1.f);
		if (mesh->vertex_normals.data != nullptr) {
			glBegin(GL_LINES);
			for (int j = 0; j < mesh->vertex.size * 3; j += 3) {
				glVertex3f(mesh->vertex.data[j], mesh->vertex.data[j + 1], mesh->vertex.data[j + 2]);
				glVertex3f(mesh->vertex.data[j] + mesh->vertex_normals.data[j], mesh->vertex.data[j + 1] + mesh->vertex_normals.data[j + 1], mesh->vertex.data[j + 2] + mesh->vertex_normals.data[j + 2]);
			}
			glEnd();
		}
	}
}

void ComponentMesh::OnInspector()
{
	ActiveImGui();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::BeginPopupContextItem("Mesh")) {
			if (ImGui::Button("Delete")) {
				object->DeleteComponent(this);
			}
			ImGui::EndPopup();
		}

		if (mesh != nullptr) {
			ImGui::Text("%i triangles (%i vertices, %i indices)", mesh->index.size / 9, mesh->vertex.size, mesh->index.size / 3);

			ImGui::Checkbox("Face Normals", &view_face_normals);
			ImGui::Checkbox("Vertex Normals", &view_vertex_normals);

			ImGui::Separator();
			
			ImGui::Checkbox("Bounding Box", &object->see_bounding_box);
		}
		else {
			if (ImGui::Button("Set Resource"))
				ImGui::OpenPopup("primitive_popup");
			if (ImGui::BeginPopup("primitive_popup")) {
				if (ImGui::Selectable("Cube")) // TODO: Do a for loop or a ImGui::Combo
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::CUBE);
				if (ImGui::Selectable("Tetrahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::TETRAHEDRON);
				if (ImGui::Selectable("Octahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::OCTAHEDRON);
				if (ImGui::Selectable("Dodecahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::DODECAHEDRON);
				if (ImGui::Selectable("Icosahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::ICOSAHEDRON);
				if (ImGui::Selectable("Sphere"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::SPHERE);
				if (ImGui::Selectable("Hemisphere"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::HEMISPHERE);
				if (ImGui::Selectable("Torus"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::TORUS);
				if (ImGui::Selectable("Cone"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::CONE);
				if (ImGui::Selectable("Cylinder"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::CYLINDER);

				ImGui::EndPopup();
			}
		}
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

	if (vertex_normals.data != nullptr) {
		glGenBuffers(1, &vertex_normals.id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_normals.id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_normals.size, vertex_normals.data, GL_STATIC_DRAW);
	}
}
