#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include <list>

class btRigidBody;
class Module;
class vec3;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
	enum class Tag { PLAYER, NITRO, CHECKPOINT, CHECKPOINT_FINISH, GHOST, WALL, NONE};

public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	vec3 GetPosition() const;
	void SetAsSensor(bool listener);
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetPos(const vec3& pos);
	void SetActive(bool active);
	Tag GetState();
	void SetState(Tag tag);

private:
	btRigidBody* body = nullptr;
	bool listener_on = false;
	Tag tag;
	bool active = true;

public:
	std::list<Module*> collision_listeners;
	bool on_collision = true;
};

#endif //__PhysBody3D_H__