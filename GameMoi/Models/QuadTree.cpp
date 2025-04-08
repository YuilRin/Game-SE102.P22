#include "QuadTree.h"
QuadTree::QuadTree(const RECT bounds, short level)
{
	this->m_bounds = bounds;
	this->m_level = level;
	this->m_children.reserve(4);
}
short QuadTree::GetIndex(const RECT& bounds)
{
	//Hàm xác định bounds có nằm hoàn toàn 1 trong 4 node con nào của quadtreenode
	short index = -1;
	//Tính điểm chia không gian thành 4 
 	float midpointX = m_bounds.left + (m_bounds.right - m_bounds.left) / 2.0f;
	float midpointY = m_bounds.bottom + (m_bounds.top - m_bounds.bottom) / 2.0f;

	//Kiểm tra bounds nằm hoàn toàn ở trên hay dưới đường chia ngang 
	bool topQuadrant = (bounds.bottom > midpointY);

	bool bottomQuadrant = (bounds.top < midpointY);

	if (bounds.right < midpointX)
	{
		//Trường hợp nằm bên trái đường dọc
		if (topQuadrant)
		{
			index = 1;//Trên trái
		}
		else if (bottomQuadrant)
		{
			index = 2;//Dưới trái 
		}
	}
	else if (bounds.left > midpointX)
	{
		if (topQuadrant)
		{
			index = 0;//Trên phải 
		}
		else if (bottomQuadrant)
		{
			index = 3;//Dưới phải
		}
	}
	return index;
}

void QuadTree::Insert(GameObject* object)
{
	//Kiểm tra bounds có nằm trong vùng của node hiện tại không
	if (!isIntersectedInGame(this->m_bounds, object->GetBounding()))
		return;

	if (!m_children.empty())//Đã có node con
	{
		short index = GetIndex(object->GetBounding());//Kiểm tra object có nằm gọn trong node không
		if (index != -1)
		{
			m_children[index]->Insert(object);
			return;
		}
	}
	m_objects.push_back(object);

	if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS)
	{
		if (m_children.empty())
			Split();

		short i = 0;
		while (i < m_objects.size())
		{
			short index = GetIndex(m_objects[i]->GetBounding());
			if (index != -1)
			{
				m_children[index]->Insert(m_objects[i]);
				m_objects.erase(m_objects.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}

}

std::vector<GameObject*> QuadTree::Retrieve(const RECT bounds)
{
	//Hàm tìm kiếm tất cả Object nằm trong vùng bounds
	vector<GameObject*> foundObjects;
	short index = GetIndex(bounds);

	if (!m_children.empty() && index != -1)
	{
		foundObjects = m_children[index]->Retrieve(bounds);
	}
	else
	{
		for (auto child : m_children)
		{
			if (isIntersectedInGame(child->m_bounds, bounds))
			{
				vector<GameObject*> childObjects = child->Retrieve(bounds);
				foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
			}
		}
	}

	foundObjects.insert(foundObjects.end(), m_objects.begin(), m_objects.end());

	return foundObjects;
}

void QuadTree::DeleteObjects()
{
	if (!m_objects.empty())
	{
		auto i = 0;
		while (i < m_objects.size())
		{
			if (m_objects[i]->getStatus() == eStatus::DESTROY)
			{
				auto obj = m_objects[i];
				m_objects.erase(m_objects.begin() + i);
				obj->Release();
				delete obj;
			}
			else
				i++;
		}
	}

	if (!m_children.empty())
		for (auto child : m_children)
			child->DeleteObjects();
}


void QuadTree::Split()
{
	const float halfWidth = (m_bounds.right - m_bounds.left) / 2.f;
	const float halfHeight = (m_bounds.top - m_bounds.bottom) / 2.f;

	RECT rect;
	//Node 0
	rect.left = m_bounds.left + halfWidth;
	rect.right = m_bounds.right;
	rect.bottom = m_bounds.bottom + halfHeight;
	rect.top = m_bounds.top;
	m_children.push_back(new QuadTree(rect, m_level + 1));
	//Node 1
	rect.left = m_bounds.left;
	rect.right = m_bounds.left + halfWidth;
	m_children.push_back(new QuadTree(rect, m_level + 1));
	//Node 2
	rect.bottom = m_bounds.bottom;
	rect.top = m_bounds.bottom + halfHeight;
	m_children.push_back(new QuadTree(rect, m_level + 1));
	//Node 3
	rect.left = m_bounds.left + halfWidth;
	rect.right = m_bounds.right;
	m_children.push_back(new QuadTree(rect, m_level + 1));
}

void QuadTree::Release()
{
	if (!m_objects.empty())
	{
		for (auto obj : m_objects)
		{
			obj->Release();
			SAFE_DELETE(obj);
		}
	}
	if (!m_children.empty())
		for (auto child : m_children)
		{
			child->Release();
			SAFE_DELETE(child);
		}
}
QuadTree* QuadTree::_instance = nullptr;

QuadTree* QuadTree::getInstance()
{
	return _instance;
}

void QuadTree::setInstance(QuadTree* root)
{
	_instance = root;
}