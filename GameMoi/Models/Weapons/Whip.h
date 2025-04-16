#pragma once
#ifndef WHIP_H
#define WHIP_H

#include "Weapon.h"
#include "../../Animation/Animation.h"


class Whip : public Weapon {
private:
    std::vector<Frame> currentFrames;
	int whipLevel;
    Animation whipAnimation;

    float whipTimer = 0.0f;
    const float whipDuration = 0.9f;

    ID3D11ShaderResourceView* texture;
    // Các offset cho từng frame của roi
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> frameOffsets;

    void UpdateHitbox(); // xử lý va chạm

public:
    Whip(float x, float y, int level, ID3D11Device* device);
    void SetLevel(int level);
    int GetLevel() {
        return whipLevel;
    };
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    void Attack() override;
};

#endif
