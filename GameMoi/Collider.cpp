#include "Collider.h"

// Constructor của Collider
Collider::Collider(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {
}

// Hàm kiểm tra va chạm giữa hai collider
bool Collider::CheckCollision(const Collider& other) const {
    return x < other.x + other.width &&
        x + width > other.x &&
        y < other.y + other.height &&
        y + height > other.y;
}
