#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include "JSON/json.hpp"

class GameObject;

enum ComponentType
{
	NONE = -1,
	TRANSFORM,
	MESH,
	MATERIAL,
	LIGHT,
	CAMERA
};

class Component
{
public:
	Component(GameObject* parent, ComponentType type);
	virtual ~Component();

	virtual void Update();
	virtual void Init();

	virtual void Enable();
	virtual void Disable();

	virtual void OnInspector() = 0;

	virtual void Save(nlohmann::json &node) { }
	virtual void Load(const nlohmann::json &node) { }

	const bool IsActive() const;
	void SetActive(bool to_active);
	void ActiveImGui(const char* checkbox_name = ""); // Just to print in UI checkbox to active/deactive the component and don't have to set is_active a public var

	const ComponentType GetType() const;


public:
	GameObject* object = nullptr;

private:
	ComponentType type = ComponentType::NONE;

	bool is_active = true;
};
#endif // __COMPONENT_H__
