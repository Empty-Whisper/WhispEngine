#include "ComponentMesh.h"
#include "Application.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/MathFunc.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MeshImporter.h"
#include "ResourceMesh.h"
#include "ComponentTransform.h"

ComponentMesh::ComponentMesh(GameObject *parent) : Component(parent, ComponentType::MESH)
{
	material = (ComponentMaterial*)parent->CreateComponent(ComponentType::MATERIAL);
}

void ComponentMesh::Update()
{
	if (uid == 0u)
		return;
	ResourceMesh* mesh = (ResourceMesh*)App->resources->Get(uid);
	if (mesh == nullptr)
		return;

	glPushMatrix();
	glMultMatrixf(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix().Transposed().ptr());

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
		DrawWireFrame(mesh);
	}
	if (object->GetSelect() != ObjectSelected::NONE)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		DrawOutline(mesh);
	}
	if(App->renderer3D->fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		Draw(mesh);
	}
	DrawNormals(mesh);
	glColor3f(0.f, 0.f, 0.f);

	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

ComponentMesh::~ComponentMesh()
{
	App->resources->FreeMemory(uid);

	if (object->GetComponent(ComponentType::MATERIAL) != nullptr) {
		if (material != nullptr) {
			object->components.erase(std::find(object->components.begin(), object->components.end(), material));
			delete material;
			material = nullptr;
		}
	}
}

void ComponentMesh::Draw(const ResourceMesh* mesh)
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

void ComponentMesh::DrawWireFrame(const ResourceMesh* mesh) {
	glDisable(GL_LIGHTING);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index.id);
	glDrawElements(GL_TRIANGLES, mesh->index.size, GL_UNSIGNED_INT, NULL);
	glEnable(GL_LIGHTING);
}

void ComponentMesh::DrawOutline(const ResourceMesh* mesh)
{
	if (glIsEnabled(GL_STENCIL_TEST))
	{

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		// Render the mesh into the stencil buffer.

		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		Draw(mesh);

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

		DrawWireFrame(mesh);


		/*--------------------------------------*/
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisableClientState(GL_VERTEX_ARRAY);
		glLineWidth(1);
	}

}

void ComponentMesh::DrawNormals(const ResourceMesh* mesh)
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

		if (uid != 0U) {
			ResourceMesh* mesh = (ResourceMesh*)App->resources->Get(uid);
			if (mesh != nullptr) {
				ImGui::Text("%i triangles (%i vertices, %i indices)", mesh->index.size / 9, mesh->vertex.size, mesh->index.size / 3);

				ImGui::Checkbox("Face Normals", &view_face_normals);
				ImGui::Checkbox("Vertex Normals", &view_vertex_normals);

				ImGui::Separator();

				ImGui::Checkbox("Bounding Box", &object->see_bounding_box);
			}
		}
		else {
			if (ImGui::Button("Set Resource"))
				ImGui::OpenPopup("primitive_popup");
			if (ImGui::BeginPopup("primitive_popup")) {
				/*if (ImGui::Selectable("Cube")) // TODO: Do a for loop or a ImGui::Combo
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::CUBE, this);
				if (ImGui::Selectable("Tetrahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::TETRAHEDRON, this);
				if (ImGui::Selectable("Octahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::OCTAHEDRON, this);
				if (ImGui::Selectable("Dodecahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::DODECAHEDRON, this);
				if (ImGui::Selectable("Icosahedron"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::ICOSAHEDRON, this);
				if (ImGui::Selectable("Sphere"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::SPHERE, this);
				if (ImGui::Selectable("Hemisphere"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::HEMISPHERE, this);
				if (ImGui::Selectable("Torus"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::TORUS, this);
				if (ImGui::Selectable("Cone"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::CONE, this);
				if (ImGui::Selectable("Cylinder"))
					mesh = App->object_manager->CreateMeshPrimitive(Primitives::CYLINDER, this);
					*/
				ImGui::EndPopup();
			}
		}
	}
}

AABB ComponentMesh::GetAABB() const
{
	return aabb;
}

void ComponentMesh::CalulateAABB_OBB()
{
	obb = local_box;
	obb.Transform(((ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM))->GetGlobalMatrix());

	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
}

OBB ComponentMesh::GetOBB() const
{
	return obb;
}

void ComponentMesh::Save(nlohmann::json & node)
{
	node["MeshId"] = uid;
}

void ComponentMesh::Load(const nlohmann::json & node)
{
	uid = node["MeshId"];
	App->resources->LoadToMemory(uid);
}