#include "Octree.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleObjectManager.h"

OctreeTree::OctreeTree(const AABB & boundary, const int & capacity)
{
	root = new OctreeNode(boundary, capacity, nullptr);
}

OctreeTree::OctreeTree(const float3 & minPoint, const float3 & maxPoint, const int & capacity) 
{
	root = new OctreeNode(AABB(minPoint, maxPoint), capacity, nullptr);
}

OctreeTree::OctreeTree(const int & capacity)
{
	root = new OctreeNode(AABB(), capacity, nullptr);
}

OctreeTree::~OctreeTree()
{
	if (root != nullptr)
		delete root;
}

void OctreeTree::Insert(GameObject * obj)
{
	if (!obj->IsStatic())
		return;
	ComponentMesh* mesh = nullptr;
	if (obj->TryGetComponent(ComponentType::MESH, (Component*&)mesh)) {
		if (root != nullptr) {
			if (!root->section.Contains(mesh->GetAABB())) {
				LOG("Object %s not inside boundary min(%.2f, %.2f, %.2f) max(%.2f, %.2f, %.2f)", obj->GetName(), root->section.MinX(), root->section.MinY(), root->section.MinZ(), root->section.MaxX(), root->section.MaxY(), root->section.MaxZ());
				Recalculate();
				return;
			}
			root->Insert(obj);
		}
	}
}

void OctreeTree::Remove(GameObject * obj)
{
	root->Remove(obj);
	Recalculate();
}

void OctreeTree::Render() const
{
	root->Render();
}

void OctreeTree::Recalculate()
{
	Clear();
	std::vector<GameObject*> objects;
	root->section = App->object_manager->GetMaxAABB(App->object_manager->GetRoot(), objects);

	for (auto object = objects.begin(); object != objects.end(); object++) {
		Insert(*object);
	}
}

void OctreeTree::Clear()
{
	if (root != nullptr) {
		for (auto child = root->children.begin(); child != root->children.end(); child++)
			delete *child;
		root->children.clear();
		root->is_leaf = true;
	}
}

OctreeNode::OctreeNode(const AABB & section, const int & capacity, OctreeNode* parent)
{
	this->section = section;
	this->capacity = capacity;
	this->parent = parent;
}

OctreeNode::~OctreeNode()
{
	for (auto child = children.begin(); child != children.end(); ++child) {
		delete *child;
		*child = nullptr;
	}
}

bool OctreeNode::Insert(GameObject * obj)
{
	if (!section.Contains(((ComponentMesh*)obj->GetComponent(ComponentType::MESH))->GetAABB()))
		return false;

	if (is_leaf) { // if has not more nodes
		if (objects.size() < capacity) { // will check if there is space to new objects
			objects.push_back(obj);
			return true;
		}
		else {							 // if not
			RecalculateNode();				// will recalculate its objects

			Subdivide();					// and subdivide, creating new 8 child nodes

		}
	}
	
	for (auto child = children.begin(); child != children.end(); child++) { // if has childs will check if fits in its sections
		if ((*child)->Insert(obj))
			return true;
	}
	objects.push_back(obj); // if not fits in any child will add to main node
	return true;
}

void OctreeNode::RecalculateNode()
{
	std::vector<GameObject*> refactor_objects; // will keep all node objects to later Inserts again
	StealAllObjects(refactor_objects);

	for (auto node = children.begin(); node != children.end(); ++node) {
		delete *node;
	}
	children.clear();
	is_leaf = true;
	for (auto object = refactor_objects.begin(); object != refactor_objects.end(); ++object) {
		Insert(*object);
	}
}

void OctreeNode::StealAllObjects(std::vector<GameObject *> &refactor_objects)
{
	for (auto object = objects.begin(); object != objects.end(); ++object) {
		refactor_objects.push_back(*object);
	}
	objects.clear();

	for (auto node = children.begin(); node != children.end(); ++node) {
		(*node)->StealAllObjects(refactor_objects);
	}
}

bool OctreeNode::Remove(GameObject * obj)
{
	for (auto object = objects.begin(); object != objects.end(); ++object) {
		if (obj == *object) {
			objects.erase(object);
			RecalculateNode();
			return true;
		}
	}
	if(!is_leaf)
		for (auto child = children.begin(); child != children.end(); ++child) {
			if ((*child)->Remove(obj))
				return true;
		}
	return false;
}

void OctreeNode::Render() const
{
	section.Draw(1.f, 0.f, 0.f);
	for (auto child = children.begin(); child != children.end(); child++)
		(*child)->Render();
}

void OctreeNode::Subdivide()
{
	float3 center = section.CenterPoint();

	/*AABB up_ne = AABB(center, section.maxPoint);
	AABB up_nw = AABB(float3(section.MinX(), center.y, center.z), float3(center.x, section.MaxY(), section.MaxZ()));
	AABB up_se = AABB(float3(center.x, center.y, section.MinZ()), float3(section.MaxX(), section.MaxY(), center.z));
	AABB up_sw = AABB(float3(section.MinX(), center.y, section.MinZ()), float3(center.x, section.MaxY(), center.z));

	AABB down_ne = AABB(float3(center.x, section.MinY(), center.z), float3(section.MaxX(), center.y, section.MaxZ()));
	AABB down_nw = AABB(float3(section.MinX(), section.MinY(), center.z), float3(center.x, center.y, section.MaxZ()));
	AABB down_se = AABB(float3(center.x, section.MinY(), section.MinZ()), float3(section.MaxX(), center.y, center.z));
	AABB down_sw = AABB(section.minPoint, center);*/

	children.push_back(new OctreeNode(AABB(center, section.maxPoint), capacity, this));
	children.push_back(new OctreeNode(AABB(float3(section.MinX(), center.y, center.z), float3(center.x, section.MaxY(), section.MaxZ())), capacity, this));
	children.push_back(new OctreeNode(AABB(float3(center.x, center.y, section.MinZ()), float3(section.MaxX(), section.MaxY(), center.z)), capacity, this));
	children.push_back(new OctreeNode(AABB(float3(section.MinX(), center.y, section.MinZ()), float3(center.x, section.MaxY(), center.z)), capacity, this));
	children.push_back(new OctreeNode(AABB(float3(center.x, section.MinY(), center.z), float3(section.MaxX(), center.y, section.MaxZ())), capacity, this));
	children.push_back(new OctreeNode(AABB(float3(section.MinX(), section.MinY(), center.z), float3(center.x, center.y, section.MaxZ())), capacity, this));
	children.push_back(new OctreeNode(AABB(float3(center.x, section.MinY(), section.MinZ()), float3(section.MaxX(), center.y, center.z)), capacity, this));
	children.push_back(new OctreeNode(AABB(section.minPoint, center), capacity, this));

	is_leaf = false;
}
