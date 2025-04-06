#ifndef COLLIDER_H
#define COLLIDER_H
#include <vector>

class Collider {
public:
    float x, y;          // Tọa độ góc trên bên trái của collider
    float width, height; // Chiều rộng và chiều cao của collider

    // Constructor
    Collider(float x, float y, float width, float height);

    // Hàm kiểm tra va chạm giữa hai collider
    bool CheckCollision(const Collider& other) const;

    // Getter
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }

    static std::vector<Collider> CreateCollisionObjects(std::vector<std::vector<int>>& tileMap, int TILE_SIZE);
};

#endif // COLLIDER_H
