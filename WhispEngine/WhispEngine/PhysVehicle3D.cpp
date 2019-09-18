#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	Cube cabin(2.25F, 0.75F, 1.5);
	Cube false_down_chassis(2.5F, 0.75F, 4);

	chassis.color.Set(1.0F, 0.0F, 0.0F);
	cabin.color.Set(1.0F, 0.0F, 0.0F);
	false_down_chassis.color.Set(1.0F, 0.0F, 0.0F);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&false_down_chassis.transform);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	cabin.transform.M[12] += offset.getX();
	cabin.transform.M[13] += offset.getY() + chassis.size.y;
	cabin.transform.M[14] += offset.getZ();

	false_down_chassis.transform.M[12] += offset.getX();
	false_down_chassis.transform.M[13] += offset.getY() - 0.5F;
	false_down_chassis.transform.M[14] += offset.getZ();

	


	if (this->GetState() == Tag::PLAYER) {
		Cube nitro_indicator(0.5F, 0.5F, 0.5F);

		nitro_indicator.color.Set(nitro_off, nitro_on, 0);

		vehicle->getChassisWorldTransform().getOpenGLMatrix(&nitro_indicator.transform);
				
		nitro_indicator.transform.M[12] += offset.getX();
		nitro_indicator.transform.M[13] += offset.getY() + 2;
		nitro_indicator.transform.M[14] += offset.getZ();

		chassis.Render();
		cabin.Render();
		false_down_chassis.Render();
		nitro_indicator.Render();
	}
	else if (GetState() == Tag::GHOST) {

		/*wheel.radius = 0.6F;
		wheel.height = 0.7F;
		for (int i = 0; i < 4; ++i)
		{
			if (i == 0)
				wheel.SetPos(chassis.transform.M[12] += 1, 0.6F, chassis.transform.M[14] += 1);
			else if (i == 1)
				wheel.SetPos(chassis.transform.M[12] += 1, 0.6F, chassis.transform.M[14] -= 1);
			else if (i == 2)
				wheel.SetPos(chassis.transform.M[12] -= 1, 0.6F, chassis.transform.M[14] += 1);
			else if (i == 3)
				wheel.SetPos(chassis.transform.M[12] -= 1, 0.6F, chassis.transform.M[14] -= 1);

			wheel.Render();
		}*/

		chassis.color.Set(0.0F, 1.0F, 0.75F);
		cabin.color.Set(0.0F, 1.0F, 0.75F);
		false_down_chassis.color.Set(0.0F, 1.0F, 0.75F);

		chassis.Render();
		cabin.Render();
		false_down_chassis.Render();
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::GetForwardVector() const
{
	btVector3 f_vector = vehicle->getForwardVector();
	return vec3(f_vector.getX(), f_vector.getY(), f_vector.getZ());
}

vec3 PhysVehicle3D::GetUpperVector() const
{
	btVector3 u_vector = vehicle->getUpVector();
	return vec3(u_vector.getX(), u_vector.getY(), u_vector.getZ());
}

//void PhysVehicle3D::DisableWheelCollision(bool is_true)
//{
//		if (is_true == true)
//		{
//			vehicle->getNumWheels();
//			//this->vehicle->getWheelInfo(0).;
//			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
//		}
//		else
//		{
//			body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
//		}
//	}
//}
