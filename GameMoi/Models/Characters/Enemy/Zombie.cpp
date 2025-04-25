#include "Zombie.h"
#include <WICTextureLoader.h>
#include <stdio.h>
#include "../../../Utilities/GameInit.h"
#include "../Player/Player.h"

Zombie::Zombie(float x, float y, ID3D11ShaderResourceView* texture)
    : Enemy(x, y, texture) {

    // Thiết lập thông số cho Zombie
    _health = 50;
    _damage = 15;
    _moveSpeed = 30.0f;
    _enemyType = eID::ZOMBIE;
    _isBoss = false;
    _detectionRange = 150.0f;

    // Khởi tạo timer cho chuyển hướng ngẫu nhiên
    _movementTimer = new StopWatch();

    // Đặt trạng thái mặc định
    SetState(eStatus::NORMAL);
}

Zombie::~Zombie() {
    SAFE_DELETE(_movementTimer);
}

void Zombie::Update(float elapsedTime) {
    Enemy::Update(elapsedTime);
    if (!_isActive || IsDead()) return;

    // Vật lý rơi
    if (!isOnGround) {
        velocityY += 9.8f * elapsedTime;
        y += velocityY;
    }

    // Xử lý va chạm mặt đất giống player
    float ySan = (x >= 390) ? 290.0f : 340.0f;
    if (y >= ySan) {
        y = ySan;
        velocityY = 0;
        isOnGround = true;
    }
    else {
        isOnGround = false;
    }

    // Luôn dí theo player trên trục X
    float _playerX = _player->GetX();
    DetectPlayer(_playerX, 0);

    // Di chuyển ngang
    if (_status == eStatus::MOVING_LEFT) {
        x -= _moveSpeed * elapsedTime;
    }
    else if (_status == eStatus::MOVING_RIGHT) {
        x += _moveSpeed * elapsedTime;
    }
}


void Zombie::LoadAnimations(ID3D11Device* device) {
    ID3D11ShaderResourceView* spriteSheet = nullptr;

    DirectX::CreateWICTextureFromFile(
        device,
        L"Image/zombie.png",
        nullptr,
        &spriteSheet
    );

    // Chỉ dùng 2 frame di chuyển
    std::vector<Frame> walkFrames = {
     {32, 64, 64, 128},   // Frame 1
     {0, 64, 32, 128}     // Frame 2
    };

    // Dùng chung cho mọi trạng thái
    _animations[eStatus::NORMAL] = new Animation(spriteSheet, walkFrames, 0.2f);
    _animations[eStatus::MOVING_LEFT] = new Animation(spriteSheet, walkFrames, 0.15f);
    _animations[eStatus::MOVING_RIGHT] = new Animation(spriteSheet, walkFrames, 0.15f);
    _animations[eStatus::ATTACKING] = new Animation(spriteSheet, walkFrames, 0.15f);
    _animations[eStatus::DIE] = new Animation(spriteSheet, walkFrames, 0.15f);
}


void Zombie::DetectPlayer(float playerX, float /*playerY*/) {
    float distX = playerX - x;
    if (distX < -5.0f) {
        SetState(eStatus::MOVING_LEFT);
        _direction = eDirection::LEFT;
    }
    else if (distX > 5.0f) {
        SetState(eStatus::MOVING_RIGHT);
        _direction = eDirection::RIGHT;
    }
    else {
        SetState(eStatus::NORMAL);
    }
}

void Zombie::ChangeDirection() {
    int direction = rand() % 3;
    switch (direction) {
    case 0: SetState(eStatus::NORMAL); break;
    case 1:
        SetState(eStatus::MOVING_LEFT);
        _direction = eDirection::LEFT;
        break;
    case 2:
        SetState(eStatus::MOVING_RIGHT);
        _direction = eDirection::RIGHT;
        break;
    }
}

void Zombie::SetGroundColliders(const std::vector<Collider*>& colliders) {
    groundColliders = colliders; // Now types match
}