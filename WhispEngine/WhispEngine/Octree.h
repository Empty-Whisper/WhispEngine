#pragma once

#include <vector>
#include "MathGeoLib/include/Geometry/AABB.h"

class GameObject;
class OctreeNode;

class OctreeTree
{
public:
	OctreeTree(const AABB &boundary, const int& capacity);
	OctreeTree(const float3 &minPoint, const float3 &maxPoint, const int& capacity);
	OctreeTree(const int& capacity = 4);
	~OctreeTree();

public:
	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void Render() const;

	void Clear();

	void Create(const AABB &limits);

	bool Intersect(std::vector<GameObject*>& objects, int primitive);

private:
	OctreeNode* root = nullptr;
};

class OctreeNode
{
	friend class OctreeTree;
private:	// Private constructors because we only want to create nodes inside Tree class
	OctreeNode(const AABB &section, const int &capacity, OctreeNode* parent);
	~OctreeNode();

private:
	bool Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void Render() const;
	void Subdivide();

private:
	std::vector<GameObject*> objects;
	OctreeNode* parent = nullptr;
	std::vector<OctreeNode*> children;
	AABB section;

	int capacity = 0;
	bool is_leaf = true;
};
