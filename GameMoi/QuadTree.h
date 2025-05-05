#pragma once
#include "Utilities/define.h"
#include "utils.h"
#include "Models/GameObject.h"
#include <vector>

class QuadTree
{
public:
    QuadTree(const RECT& bounds, short level = 0);
    ~QuadTree() {};

    std::vector<GameObject*> Retrieve(const RECT bounds);    // Truy vấn các object giao với vùng bounds
    void Insert(GameObject* object);                          // Thêm object vào quadtree
    void DeleteObjects();                                     // Xoá object có trạng thái DESTROY
    void Release();                                           // Giải phóng tất cả memory

    static QuadTree* getInstance();                           // Singleton getter
    static void setInstance(QuadTree* root);                  // Singleton setter

protected:
    static QuadTree* _instance;                               // Singleton instance

    short m_level;                                            // Cấp độ hiện tại của node
    RECT m_bounds;                                            // Vùng bao (bound) của node

    std::vector<GameObject*> m_objects;                       // Các object nằm trong node hiện tại
    std::vector<QuadTree*> m_children;                        // 4 node con sau khi split

    short GetIndex(const RECT& bounds);                       // Xác định vùng con tương ứng
    void Split();                                             // Chia node thành 4 phần
};
