#include "ComponentMaterial.h"
#include "GameObject.h"
#include "ComponentMesh.h"



ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent, ComponentType::MATERIAL)
{
	((ComponentMesh*)parent->GetComponent(ComponentType::MESH))->SetMaterial(this);
}


ComponentMaterial::~ComponentMaterial()
{
}

const bool ComponentMaterial::HasTexture() const
{
	return false;
}

const uint ComponentMaterial::GetIDTexture() const
{
	return id_tex;
}

void ComponentMaterial::SetFaceColor(const float & r, const float & g, const float & b, const float & a)
{
	face_color[0] = r;
	face_color[1] = g;
	face_color[2] = b;
	face_color[3] = a;
}

void ComponentMaterial::SetFaceColor(const float *& c)
{
	memcpy(face_color, c, sizeof(float) * 4);
}

void ComponentMaterial::SetWireColor(const float & r, const float & g, const float & b, const float & a)
{
	wire_color[0] = r;
	wire_color[1] = g;
	wire_color[2] = b;
	wire_color[3] = a;
}

void ComponentMaterial::SetWireColor(const float *& c)
{
	memcpy(wire_color, c, sizeof(float) * 4);
}

const float * ComponentMaterial::GetFaceColor() const
{
	return &face_color[0];
}

const float * ComponentMaterial::GetWireColor() const
{
	return &wire_color[0];
}
