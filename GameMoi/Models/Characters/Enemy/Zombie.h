#pragma once
#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"
#include "../../../Tilemap/Collider.h"
#include "../Player/Player.h" // Thêm dòng này

class Zombie : public Enemy {
private:
    Player* _player; // Sửa tên biến thành _player
    float _detectionRange;
    StopWatch* _movementTimer;
    float velocityY;
    bool isOnGround;
    std::vector<Collider*> groundColliders;

public:
    Zombie(float x, float y, ID3D11ShaderResourceView* texture = nullptr);
    virtual ~Zombie();

    virtual void Update(float elapsedTime) override;
    virtual void LoadAnimations(ID3D11Device* device) override;
    void DetectPlayer(float playerX, float playerY);
    void ChangeDirection();
    void SetGroundColliders(const std::vector<Collider*>& colliders);
    void SetPlayerReference(Player* player) { _player = player; } // Thêm phương thức này
};

#endif // __ZOMBIE_H__
