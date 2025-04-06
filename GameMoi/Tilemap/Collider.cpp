#include "Collider.h"
#include "TileMap.h"

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



// Phương thức tạo các collider từ tile map
std::vector<Collider> Collider::CreateCollisionObjects(std::vector<std::vector<int>>& tileMap, int TILE_SIZE) {
    std::vector<Collider> colliders;
    int rows = tileMap.size();
    int cols = tileMap[0].size();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (tileMap[row][col] == 0) {  // Nếu là tile sàn gạch
                // Tìm chiều rộng và chiều cao của vùng liên tiếp
                float x = col;
                float y = row;
                float width = 0;
                float height = 0;

                // Tìm chiều rộng của đối tượng va chạm (liên tiếp các ô 0 trong dòng)
                while (col + width < cols && tileMap[row][col + width] == 0) {
                    width++;
                }

                // Tìm chiều cao của đối tượng va chạm (liên tiếp các ô 0 trong cột)
                while (row + height < rows && tileMap[row + height][col] == 0) {
                    bool isValid = true;
                    for (int i = 0; i < width; ++i) {
                        if (tileMap[row + height][col + i] != 0) {
                            isValid = false;
                            break;
                        }
                    }
                    if (!isValid) break;
                    height++;
                }

                // Tạo một đối tượng va chạm hình chữ nhật từ x, y, width, height
                colliders.push_back(Collider(x * TILE_SIZE, y * TILE_SIZE, width * TILE_SIZE, height * TILE_SIZE));

                //// Đánh dấu các tile đã xử lý để tránh tạo trùng đối tượng va chạm
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        tileMap[row + i][col + j] = 22;  // Đánh dấu là đã xử lý
                    }
                }
            }
        }
    }

    return colliders;
}