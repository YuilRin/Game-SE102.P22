#pragma once
#include "Weapon.h"
#include "../../Animation/Animation.h"
#include <map>
#include <vector>

class Whip : public Weapon {
private:
    int whipLevel;
    Animation whipAnimation;
    float whipTimer;
    float whipDuration = 0.9f;  // Thời gian roi hoạt động

    // Offset cho các frame của roi (cặp first:hướng phải, second:hướng trái)
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> frameOffsets;

public:
    Whip(float x, float y, int level, ID3D11Device* device);
    void SetLevel(int level);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    void Attack() override;

    // Cập nhật hitbox dựa trên frame hiện tại của animation
    void UpdateHitbox() override;

    // Lấy kích thước hitbox hiện tại của roi
    void GetHitboxSize(float& width, float& height) const;

    // Lấy frame hiện tại để tính toán va chạm
    int GetCurrentFrameIndex() const { return whipAnimation.GetCurrentFrameIndex(); }
};