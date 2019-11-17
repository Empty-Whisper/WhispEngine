#pragma once

#include <vector>
#include "MathGeoLib/include/Geometry/AABB.h"

class GameObject;

class OctreeTree
{
public:
	OctreeTree();
	~OctreeTree();

public:
	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void Clear();

	void Create(const AABB &limits);

	bool Intersect(std::vector<GameObject*>& objects, int primitive);

private:
	OctreeNode* root = nullptr;
	int max_levels = 0;
	AABB boundary;
};

class OctreeNode
{
public:
	OctreeNode();
	~OctreeNode();

private:
	std::vector<GameObject*> objects;
	OctreeNode* parent = nullptr;
	std::vector<OctreeNode*> children;
	AABB section;
};
