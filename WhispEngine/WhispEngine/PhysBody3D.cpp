#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body), on_collision(true), active(true)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

vec3 PhysBody3D::GetPosition() const
{
	btVector3 vector = body->getWorldTransform().getOrigin();
	return vec3(vector.getX(), vector.getY(), vector.getZ());
}

void PhysBody3D::SetAsSensor(bool listener)
{
	if (this->listener_on != listener)
	{
		this->listener_on = listener;

		if (listener == true)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		else
		{
			body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

void PhysBody3D::SetPos(const vec3 &pos)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(pos.x, pos.y, pos.z));
	body->setWorldTransform(t);
}

void PhysBody3D::SetActive(bool is_active)
{
	active = is_active;
	on_collision = is_active;
}

PhysBody3D::Tag PhysBody3D::GetState()
{
	return tag;
}


void PhysBody3D::SetState(Tag current_state)
{
	this->tag = current_state;
}