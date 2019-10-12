#include "Component.h"
#include "Application.h"
#include "GameObject.h"

Component::Component(GameObject * parent, ComponentType type) : parent(parent), type(type)
{
}

Component::~Component(){}

void Component::Init()
{}


void Component::Update()
{}

void Component::Enable()
{
	if (!is_active)
		is_active = true;
}

void Component::Disable()
{
	if (is_active)
		is_active = false;
}

const bool Component::IsActive() const
{
	return is_active;
}

const ComponentType Component::GetType() const
{
	return type;
}



