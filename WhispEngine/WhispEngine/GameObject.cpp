#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Application.h"
#include "MathGeoLib/include/MathGeoLib.h"

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
	AABB aabb = mesh->GetAABB();

	float MinX = aabb.MinX();
	float MinY = aabb.MinY();
	float MinZ = aabb.MinZ();
	float MaxX = aabb.MaxX();
	float MaxY = aabb.MaxY();
	float MaxZ = aabb.MaxZ();
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor3f(0.f, 1.f, 0.f);

	glVertex3f(MinX, MinY, MinZ);
	glVertex3f(MaxX, MinY, MinZ);

	glVertex3f(MinX, MinY, MinZ);
	glVertex3f(MinX, MinY, MaxZ);

	glVertex3f(MinX, MinY, MinZ);
	glVertex3f(MinX, MaxY, MinZ);

	glVertex3f(MaxX, MinY, MaxZ);
	glVertex3f(MaxX, MinY, MinZ);

	glVertex3f(MaxX, MinY, MaxZ);
	glVertex3f(MinX, MinY, MaxZ);


	glVertex3f(MaxX, MaxY, MaxZ);
	glVertex3f(MaxX, MinY, MaxZ);

	glVertex3f(MaxX, MaxY, MaxZ);
	glVertex3f(MinX, MaxY, MaxZ);

	glVertex3f(MaxX, MaxY, MaxZ);
	glVertex3f(MaxX, MaxY, MinZ);

	glVertex3f(MinX, MaxY, MinZ);
	glVertex3f(MaxX, MaxY, MinZ);

	glVertex3f(MinX, MaxY, MinZ);
	glVertex3f(MinX, MaxY, MaxZ);

	glVertex3f(MinX, MinY, MaxZ);
	glVertex3f(MinX, MaxY, MaxZ);

	glVertex3f(MaxX, MinY, MinZ);
	glVertex3f(MaxX, MaxY, MinZ);
	glEnable(GL_LIGHTING);

	glEnd();
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

Component * GameObject::GetComponent(const ComponentType & type)
{
	for (auto i = components.cbegin(); i != components.cend(); ++i) {
		if (*i != nullptr)
			if ((*i)->GetType() == type)
				return *i;
	}
	return nullptr;
}

bool GameObject::HasComponent(const ComponentType & type)
{
	for (auto comp = components.begin(); comp != components.end(); comp++)
		if ((*comp)->GetType() == type)
			return true;

	return false;
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
	return obj_static;
}

void GameObject::SetStatic(bool to_static)
{
	obj_static = to_static;
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

bool GameObject::HasChild(GameObject * child)
{
	bool ret = false;
	for (auto it_child = children.begin(); it_child != children.end(); it_child++) {
		if ((*it_child) == child)
			ret = true;
		if (ret)
			break;
		if (!(*it_child)->children.empty())
			ret = (*it_child)->HasChild(child);
	}
	return ret;
}

bool GameObject::Save(nlohmann::json & node)
{
	bool ret = true;

	nlohmann::json object;

	object["active"] = active;
	object["UID"] = UID;
	object["name"] = name;
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


