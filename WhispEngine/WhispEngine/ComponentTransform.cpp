#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent, ComponentType::TRANSFORM)
{
}


ComponentTransform::~ComponentTransform()
{
	
}

void ComponentTransform::Update()
{
}
