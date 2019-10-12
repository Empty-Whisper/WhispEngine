#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent, ComponentType::TRANSFORM)
{
}


ComponentTransform::~ComponentTransform()
{
	
}

void ComponentTransform::PreUpdate()
{

}

math::float4x4 ComponentTransform::GetLocalMatrix() const
{
	//  Convert position(vec3), rotation(quad), scale(vec3) to matrix4x4 
	math::float4x4 localMatrix = math::float4x4::FromTRS(position, rotation, scale);
	return localMatrix;
}

math::float4x4 ComponentTransform::GetGlobalMatrix() const
{
	math::float4x4 globalMatrix = math::float4x4::identity;
	GameObject* parentObject = this->parent;
	//TODO: Set all parents on a list/vector/array/slack (i think list is the best option)
	//TODO: get GameObject parent Matrix from trasform and equal to globalMatrix * parentMatrix 
	
	return globalMatrix * GetLocalMatrix();
}
