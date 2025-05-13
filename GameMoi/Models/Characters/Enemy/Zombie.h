// Zombie.h
#pragma once

#include "Enemy.h"
#include "../../../Utilities/StopWatch.h"

class Zombie : public Enemy {
private:
    float _detectionRange;
    StopWatch* _movementTimer;

public:
    Zombie(float x, float y, ID3D11ShaderResourceView* texture);
    ~Zombie() override;

    void Update(float elapsedTime) override;
    void LoadAnimations(ID3D11Device* device);

private:
    void DetectPlayer(float playerX, float playerY);
    void ChangeDirection();
};