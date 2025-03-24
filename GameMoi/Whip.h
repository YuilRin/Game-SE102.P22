﻿#pragma once
#ifndef WHIP_H
#define WHIP_H

#include "Weapon.h"
#include "Animation.h"

class Whip : public Weapon {
private:
	int whipLevel;
    Animation whipAnimation;
    float whipTimer = 0.0f;
    const float whipDuration = 0.9f;

public:
    Whip(float x, float y, int level, ID3D11Device* device);
    void SetLevel(int level);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    void Attack() override;
};

#endif
