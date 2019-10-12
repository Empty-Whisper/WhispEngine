#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
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
	case TRANSFORM:
		ComponentTransform * comp = new ComponentTransform(this);
		components.push_back(comp);
		return comp;
		break;
	case MESH:
		ComponentMesh* comp = new ComponentMesh(this);
		components.push_back(comp);
		return comp;
		break;
	case MATERIAL:
		break;
	case LIGHT:
		break;
	default:
		LOG("Not declared Component type with id: %d", type);
		break;
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
