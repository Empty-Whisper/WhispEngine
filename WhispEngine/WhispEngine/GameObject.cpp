#include "GameObject.h"



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
