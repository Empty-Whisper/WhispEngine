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

void Component::SetActive(bool to_active)
{
	is_active = to_active;
}

void Component::ActiveImGui()
{
	ImGui::Checkbox("", &is_active);
}

const ComponentType Component::GetType() const
{
	return type;
}



