#pragma once
#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"

class Zombie : public Enemy
{
private:
    // Thuộc tính đặc trưng của Zombie
    float _detectionRange;    // Phạm vi phát hiện người chơi
    StopWatch* _movementTimer; // Hẹn giờ chuyển hướng

public:
    Zombie(float x, float y, ID3D11ShaderResourceView* texture = nullptr);
    virtual ~Zombie();

    // Override từ Enemy
    virtual void Update(float elapsedTime) override;
    virtual void LoadAnimations(ID3D11Device* device) override;

    // Phương thức riêng của Zombie
    void DetectPlayer(float playerX, float playerY);
    void ChangeDirection();
};

#endif // __ZOMBIE_H__
