#pragma once
#include"GameObject.h"
#include <algorithm>
#include <vector>
#include "../Utilities/Utils.h"
#include "../Utilities/define.h"
class QuadTree
{
public:
	QuadTree(const RECT bounds,short level=0);
	~QuadTree() {};

	std::vector<GameObject*> Retrieve(const RECT bounds);
	void Insert(GameObject* object);
	void DeleteObjects();
	void Release();

	static QuadTree* getInstance();
	static void setInstance(QuadTree* root);
protected:
	static QuadTree* _instance;

	short m_level;
	RECT m_bounds;
	std::vector<GameObject*> m_objects;
	std::vector<QuadTree*> m_children;

	short GetIndex(const RECT& bounds);

	void Split(void);
};