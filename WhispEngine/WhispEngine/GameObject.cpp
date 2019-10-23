#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject(GameObject * parent) : parent(parent)
{
	if (parent != nullptr)
		parent->children.push_back(this);

	CreateComponent(ComponentType::TRANSFORM);
	SetName("GameObject");
}

GameObject::~GameObject()
{
	for (auto comp = components.begin(); comp != components.end(); ++comp) {
		delete *comp;
	}

	for (auto i = children.begin(); i != children.end(); i++) {
		delete *i;
	}
	children.clear();
}

void GameObject::Update()
{
	for (auto i = components.begin(); i != components.end(); ++i) {
		if ((*i)->IsActive())
			(*i)->Update();
	}
}

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
	default:
		LOG("Not declared Component type with id: %d", type);
		break;
	}

	return nullptr;
}

Component * GameObject::GetComponent(const ComponentType & type)
{
	for (auto i = components.cbegin(); i != components.cend(); ++i) {
		if ((*i)->GetType() == type)
			return *i;
	}
	return nullptr;
}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(const bool & to_active)
{
	active = to_active; //TODO Set all its childs to to_active
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
