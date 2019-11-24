#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Application.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleRenderer3D.h"
#include "ModuleObjectManager.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"

GameObject::GameObject(GameObject * parent) : parent(parent)
{
	if (parent != nullptr)
		parent->children.push_back(this);

	CreateComponent(ComponentType::TRANSFORM);
	SetName("GameObject");
	UID = App->random->RandomGUID();
}

GameObject::~GameObject()
{
	for (auto comp = components.begin(); comp != components.end(); ++comp) {
		delete *comp;
		*comp = nullptr;
	}
	components.clear();

	for (auto i = children.begin(); i != children.end(); i++) {
		delete *i;
		*i = nullptr;
	}
	children.clear();

	if (this == App->object_manager->GetSelected())
		App->object_manager->SetSelected(nullptr);
}

void GameObject::Update()
{
	for (auto i = components.begin(); i != components.end(); ++i) {
		if ((*i)->IsActive())
			(*i)->Update();
	}
	if (components_to_delete.size() > 0) {
		for (auto c = components_to_delete.begin(); c != components_to_delete.end(); c++) {
			components.erase(std::find(components.begin(), components.end(), *c));
			delete *c;
		}
		components_to_delete.clear();
	}

	if (see_bounding_box) {
		DrawBoundingBoxAABB();
		DrawBoundingBoxOBB();
	}
}

// TODO: add all bounding boxes functions to mesh ---------------------------------------------------
void GameObject::DrawBoundingBoxAABB()
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);
	if (mesh == nullptr)
		return;
	glDisable(GL_LIGHTING);
	mesh->GetAABB().Draw(0.f, 1.f, 0.f);
	glEnable(GL_LIGHTING);
}

void GameObject::DrawBoundingBoxOBB()
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);
	if (mesh == nullptr)
		return;
	OBB obb = mesh->GetOBB();
	math::float3 points[8];
	obb.GetCornerPoints(points);

	glColor3f(0.f, 0.f, 1.f);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glVertex3fv(points[0].ptr());
	glVertex3fv(points[1].ptr());

	glVertex3fv(points[0].ptr());
	glVertex3fv(points[4].ptr());

	glVertex3fv(points[0].ptr());
	glVertex3fv(points[2].ptr());

	glVertex3fv(points[1].ptr());
	glVertex3fv(points[5].ptr());

	glVertex3fv(points[1].ptr());
	glVertex3fv(points[3].ptr());

	glVertex3fv(points[2].ptr());
	glVertex3fv(points[3].ptr());

	glVertex3fv(points[2].ptr());
	glVertex3fv(points[6].ptr());

	glVertex3fv(points[3].ptr());
	glVertex3fv(points[7].ptr());

	glVertex3fv(points[4].ptr());
	glVertex3fv(points[5].ptr());

	glVertex3fv(points[4].ptr());
	glVertex3fv(points[6].ptr());

	glVertex3fv(points[5].ptr());
	glVertex3fv(points[7].ptr());

	glVertex3fv(points[6].ptr());
	glVertex3fv(points[7].ptr());

	glEnable(GL_LIGHTING);

	glEnd();
}
AABB GameObject::GetAABB() const
{
	ComponentMesh* mesh = nullptr;
	if (TryGetComponent(ComponentType::MESH, (Component*&)mesh)) {
		return mesh->GetAABB();
	}

	AABB ret;
	ret.SetNegativeInfinity();
	return ret;
}
// --------------------------------------------------------------------------------------------------------------------------------

Component * GameObject::CreateComponent(const ComponentType & type)
{
	switch (type)
	{
	case TRANSFORM: {
		ComponentTransform * comp = new ComponentTransform(this);
		components.push_back(comp);
		return comp;
	}
		break;
	case MESH: {
		ComponentMesh* comp = new ComponentMesh(this);
		components.push_back(comp);
		return comp;
	}
		break;
	case MATERIAL: {
		ComponentMaterial* comp = new ComponentMaterial(this);
		components.push_back(comp);
		return comp;
	}
		break;
	case LIGHT:
		break;
	case CAMERA: {
		ComponentCamera* comp = new ComponentCamera(this);
		components.push_back(comp);
		return comp;
	}
				   break;
	default:
		LOG("Not declared Component type with id: %d", type);
		break;
	}

	return nullptr;
}

void GameObject::DeleteComponent(Component * comp)
{
	components_to_delete.push_back(comp);
}

Component * GameObject::GetComponent(const ComponentType & type) const
{
	for (auto i = components.cbegin(); i != components.cend(); ++i) {
		if (*i != nullptr)
			if ((*i)->GetType() == type)
				return *i;
	}
	return nullptr;
}

bool GameObject::TryGetComponent(const ComponentType & type, Component *& comp) const
{
	Component* component = GetComponent(type);
	if (component != nullptr) {
		comp = component;
		return true;
	}
	return false;
}

bool GameObject::HasComponent(const ComponentType & type)
{
	for (auto comp = components.begin(); comp != components.end(); comp++)
		if ((*comp)->GetType() == type)
			return true;
	
	return false;
}

void GameObject::Raycast(const LineSegment &ray_cast, bool &intersect, float &length)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);
	ComponentTransform* ctrans = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);

	length = 1e10;
	intersect = false;

	if (mesh != nullptr && mesh->GetAABB().IsFinite())
	{
		AABB aabb = mesh->aabb;
		ResourceMesh* resource_mesh = (ResourceMesh*)App->resources->Get(mesh->uid);

		if (ray_cast.Intersects(aabb) && resource_mesh != nullptr)
		{
			Triangle triangle_plane;
			uint* indices = resource_mesh->index.data;
			float* vertices = resource_mesh->vertex.data;
			for (int i = 0; i < resource_mesh->index.size / 3;)
			{
				triangle_plane.a = (float3)&vertices[(3 * indices[i])], vertices[(indices[i] * 3) + 1u], &vertices[(indices[i] * 3) + 2u];
				++i;
				triangle_plane.b = (float3)&vertices[(3 * indices[i])], vertices[(indices[i] * 3) + 1u], &vertices[(indices[i] * 3) + 2u];
				++i;
				triangle_plane.c = (float3)&vertices[(3 * indices[i])], vertices[(indices[i] * 3) + 1u], &vertices[(indices[i] * 3) + 2u];
				++i;

				//This is used to do not change the origianl values with the Intesect reference function
				Triangle new_triangle_plane;
				float new_length;
				float3 hit_point;

				new_triangle_plane = triangle_plane;

				LineSegment local_ray_cast(ray_cast);
				local_ray_cast.Transform(ctrans->GetGlobalMatrix().Inverted());

				if (local_ray_cast.Intersects(new_triangle_plane, &new_length, &hit_point) && new_length < length)
				{
					length = new_length;
					intersect = true;
				}
			}
		}
	}

}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(const bool & to_active)
{
	active = to_active;
}

bool GameObject::IsStatic() const
{
	return is_static;
}

void GameObject::SetStatic(bool to_static)
{
	is_static = to_static;
}

const char * GameObject::GetName() const
{
	return name.data();
}

void GameObject::SetName(const char * name)
{
	this->name.assign(name);
}


ObjectSelected GameObject::GetSelect() const
{
	return obj_selected;
}

void GameObject::Select()
{
	obj_selected = ObjectSelected::SELECTED;

	if (parent != nullptr)
	{
		if (parent->obj_selected != ObjectSelected::NONE)
			obj_selected = ObjectSelected::CHILD_FROM_PARENT_SELECTED;		
	}
	
	for (auto i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Select();
	}
}

void GameObject::Deselect()
{
	obj_selected = ObjectSelected::NONE;

	for (auto i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Deselect();
	}
}

void GameObject::Detach()
{
	parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
	parent = nullptr;
}

void GameObject::Attach(GameObject * parent)
{
	this->parent = parent;
	parent->children.push_back(this);
}

bool GameObject::HasAnyStaticChild() const
{
	for (auto i = children.begin(); i != children.end(); i++) {
		if ((*i)->IsStatic())
			return true;
		if (!(*i)->children.empty())
			if ((*i)->HasAnyStaticChild())
				return true;
	}
	return false;
}

bool GameObject::HasDynamicParent(std::vector<GameObject*>& parents) const
{
	if (parent != nullptr && parent != App->object_manager->GetRoot()) {
		if (!parent->IsStatic()) {
			parents.push_back(parent);
			parent->HasDynamicParent(parents);
			return true;
		}
		if (parent->HasDynamicParent(parents))
			return true;
	}
	return false;
}

bool GameObject::HasChild(GameObject * child) const
{
	for (auto it_child = children.begin(); it_child != children.end(); it_child++) {
		if ((*it_child) == child)
			return true;
		if (!(*it_child)->children.empty())
			if ((*it_child)->HasChild(child))
				return true;
	}
	return false;
}

bool GameObject::Save(nlohmann::json & node)
{
	bool ret = true;

	nlohmann::json object;

	object["active"] = active;
	object["UID"] = UID;
	object["name"] = name;
	object["static"] = is_static;
	GetComponent(ComponentType::TRANSFORM)->Save(object);

	nlohmann::json component;
	for (auto i = components.begin(); i != components.end(); ++i) {
		if ((*i)->GetType() != ComponentType::TRANSFORM) {
			nlohmann::json comp;
			comp["active"] = (*i)->IsActive();
			comp["type"] = (int)(*i)->GetType();
			(*i)->Save(comp);
			component.push_back(comp);
		}
	}
	object["Components"] = { component };

	object["Children"] = {};
	for (auto i = children.begin(); i != children.end(); ++i) {
		(*i)->Save(object["Children"]);
	}

	node.push_back(object);

	return ret;
}


