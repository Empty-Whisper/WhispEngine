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

	void Recalculate(const AABB& new_limit, GameObject* root);

	void Clear();

	template<typename TYPE>
	bool Intersect(std::vector<GameObject*>& objects, const TYPE & primitive) const;

private:
	OctreeNode* root = nullptr;
};

template<typename TYPE>
bool OctreeTree::Intersect(std::vector<GameObject*>& to_fill, const TYPE &primitive) const // Definition in .h to fix linker problems
{
	return root->Intersect(to_fill, primitive);
}

class OctreeNode
{
	friend class OctreeTree;
private:	// Private constructors because we only want to create nodes inside Tree class
	OctreeNode(const AABB &section, const int &capacity, OctreeNode* parent);
	~OctreeNode();

private:
	bool Insert(GameObject* obj);
	void RecalculateNode();
	/*Get All children objects deleting form corresponding nodes*/
	void StealAllObjects(std::vector<GameObject *> &refactor_objects);
	bool Remove(GameObject* obj);

	void Render() const;
	void Subdivide();

	template<typename TYPE>
	bool Intersect(std::vector<GameObject*>& objects, const TYPE & primitive) const;

private:
	std::vector<GameObject*> objects;
	OctreeNode* parent = nullptr;
	std::vector<OctreeNode*> children;
	AABB section;

	int capacity = 0;
	bool is_leaf = true;
};

template<typename TYPE>
inline bool OctreeNode::Intersect(std::vector<GameObject*>& to_fill, const TYPE & primitive) const
{
	if (primitive.Intersects(section))
	{
		for (std::vector<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->GetAABB()))
				to_fill.push_back(*it);
		}
		for (auto i = children.begin(); i != children.end(); ++i)
			if (*i != nullptr) (*i)->Intersect(to_fill, primitive);
	}
	return !to_fill.empty();
}
