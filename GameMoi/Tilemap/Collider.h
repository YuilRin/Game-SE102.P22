// Collider.h
#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>
#include <algorithm>

class Collider {
public:
    float x, y;          // Vị trí góc trên bên trái
    float width, height; // Kích thước
    float vx, vy;        // Vận tốc
    bool isBlocking;     // Có chặn vật khác hay không
    virtual ~Collider() {};

    Collider(float x, float y, float width, float height, float vx = 0, float vy = 0, bool isBlocking = true);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom) const;
    void GetSpeed(float& out_vx, float& out_vy) const;
    bool IsBlocking() const;
    void SetPosition(float newX, float newY);
    void GetPosition(float& out_x, float& out_y) const;
};

struct CollisionEvent {
    float t, nx, ny;
    float dx, dy;
    Collider* src;
    Collider* dest;
    bool isDeleted = false;

    CollisionEvent(float t, float nx, float ny, float dx, float dy, Collider* src, Collider* dest);
    static bool compare(const CollisionEvent* a, const CollisionEvent* b);
    bool WasCollided() const;
};

class CollisionManager {
private:
    static CollisionManager* instance;
    CollisionManager() {}

public:
    static CollisionManager* GetInstance();

    static void SweptAABB(
        float ml, float mt, float mr, float mb,
        float dx, float dy,
        float sl, float st, float sr, float sb,
        float& t, float& nx, float& ny);

    CollisionEvent* SweptAABB(Collider* src, float dt, Collider* dest);

    void Scan(Collider* src, float dt, const std::vector<Collider*>& objects, std::vector<CollisionEvent*>& outEvents);

    void Filter(
        Collider* src,
        std::vector<CollisionEvent*>& events,
        CollisionEvent*& colX,
        CollisionEvent*& colY,
        int filterBlock = 1,
        int filterX = 1,
        int filterY = 1);

    void Process(Collider* src, float dt, std::vector<Collider*>& objects);
};

std::vector<Collider*> CreateOptimizedCollidersFromTileMap(std::vector<std::vector<int>>& tileMap, int tileSize, int solidTileValue = 0);
std::vector<Collider*> CreateStairCollidersFromTileMap(const std::vector<std::vector<int>>& tileMap, int tileSize);

#endif // COLLIDER_H