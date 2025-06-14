#include "Models/Characters/Player/Player.h"
class Player2 : public Player {
private:
    // Có thể có các thuộc tính riêng cho Player2 nếu cần
    Vector2 spawnPosition;

public:
    Player2(string startPos, std::map<PlayerState, Animation> anims, ID3D11Device* device);
    ~Player2() = default;

    // Override input handling cho Player2 với bộ phím điều khiển khác
    void onKeyPressed(WPARAM key);
    void onKeyReleased(WPARAM key);

    // Có thể override render để có màu sắc khác biệt
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    // Reset về vị trí spawn
    void ResetToSpawn();
    void SetSpawnPosition(float x, float y);
};
