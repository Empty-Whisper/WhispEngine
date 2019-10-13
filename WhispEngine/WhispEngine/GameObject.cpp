#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject(GameObject * parent) : parent(parent)
{
	if (parent != nullptr)
		parent->children.push_back(this);

	CreateComponent(ComponentType::TRANSFORM);
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
