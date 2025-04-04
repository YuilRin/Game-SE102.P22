#pragma once
#ifndef AXE_H
#define AXE_H

#include "Weapon.h"
#include "../../Animation/Animation.h"

class Axe : public Weapon {
private:
    Animation axeAnimation;
    bool facingLeft;
    float axeTimer = 0.0f;
    const float axeDuration = 0.6f;  // Giữ axe lâu hơn whip
    float velocityX, velocityY, gravity;
    bool isThrown = false;
private:
    float attackDelay = 0.7f; 
    float delayTimer = 0.0f;   // Biến đếm thời gian

    

public:
    Axe(float x, float y, ID3D11Device* device,bool facingLeft);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    void Attack() override;
    void ResetVelocity();
    
    void SetThrown(bool thrown);
    bool IsThrown();
};

#endif
