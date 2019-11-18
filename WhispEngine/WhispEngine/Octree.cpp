#include "Octree.h"
#include "GameObject.h"
#include "ComponentMesh.h"

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
				return;
			}
			root->Insert(obj);
		}
	}
}

void OctreeTree::Remove(GameObject * obj)
{
}

void OctreeTree::Render() const
{
	root->Render();
}

void OctreeTree::Clear()
{
	if (root != nullptr) {
		for (auto child = root->children.begin(); child != root->children.end(); child++)
			delete *child;
		root->children.clear();
	}
}

void OctreeTree::Create(const AABB & limits)
{
}

bool OctreeTree::Intersect(std::vector<GameObject*>& objects, int primitive)
{
	return false;
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
	if(!section.Contains(((ComponentMesh*)obj->GetComponent(ComponentType::MESH))->GetAABB()))
		return false;
	if (objects.size() < capacity) {
		objects.push_back(obj);
		return true;
	}
	else {
		if (is_leaf) {
			Subdivide();
		}
		for (auto child = children.begin(); child != children.end(); child++) {
			if ((*child)->Insert(obj))
				return true;
		}
	}
	return false;
}

void OctreeNode::Remove(GameObject * obj)
{
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

	AABB up_ne = AABB(center, section.maxPoint);
	AABB up_nw = AABB(float3(section.MinX(), center.y, center.z), float3(center.x, section.MaxY(), section.MaxZ()));
	AABB up_se = AABB(float3(center.x, center.y, section.MinZ()), float3(section.MaxX(), section.MaxY(), center.z));
	AABB up_sw = AABB(float3(section.MinX(), center.y, section.MinZ()), float3(center.x, section.MaxY(), center.z));

	AABB down_ne = AABB(float3(center.x, section.MinY(), center.z), float3(section.MaxX(), center.y, section.MaxZ()));
	AABB down_nw = AABB(float3(section.MinX(), section.MinY(), center.z), float3(center.x, center.y, section.MaxZ()));
	AABB down_se = AABB(float3(center.x, section.MinY(), section.MinZ()), float3(section.MaxX(), center.y, center.z));
	AABB down_sw = AABB(section.minPoint, center);

	children.push_back(new OctreeNode(up_ne, capacity, this));
	children.push_back(new OctreeNode(up_nw, capacity, this));
	children.push_back(new OctreeNode(up_se, capacity, this));
	children.push_back(new OctreeNode(up_sw, capacity, this));
	children.push_back(new OctreeNode(down_ne, capacity, this));
	children.push_back(new OctreeNode(down_nw, capacity, this));
	children.push_back(new OctreeNode(down_se, capacity, this));
	children.push_back(new OctreeNode(down_sw, capacity, this));

	is_leaf = false;
}
