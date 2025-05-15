#include "Zombie.h"
#include <WICTextureLoader.h>
#include <stdio.h>
#include "../../../Utilities/GameInit.h"
#include "../Player/Player.h"
#include "../../World.h"

Zombie::Zombie(float x, float y, ID3D11ShaderResourceView* texture)
    : Enemy(x, y, texture) {

    // Initialize zombie-specific properties
    collider = new Collider(x, y, 32, 64);

    // Set zombie health and other properties using the parent's _info
    _info->SetEnemyHitPoint(2);
    _info->SetHeart(1);

    _moveSpeed = 30.0f;
    _enemyType = eID::ZOMBIE;
    _isBoss = false;
    _detectionRange = 150.0f;

    // Initialize timer for random direction changes
    _movementTimer = new StopWatch();

    // Set default state
    SetState(eStatus::NORMAL);
}

Zombie::~Zombie() {
    SAFE_DELETE(_movementTimer);
    // Parent's destructor will handle _info
}

void Zombie::Update(float elapsedTime) {
    Enemy::Update(elapsedTime);
    _velocity.y += 9.8f * elapsedTime + 1.0f;

    collider->vx = _velocity.x;
    collider->vy = _velocity.y;
    HandleCollision(elapsedTime);

    // Player tracking on X axis
    if (_player) {  // Make sure _player is not null
        float _playerX = _player->GetX();
        DetectPlayer(_playerX, 0);

        // Horizontal movement
        if (_status == eStatus::MOVING_LEFT) {
            _velocity.x -= _moveSpeed * elapsedTime;
        }
        else if (_status == eStatus::MOVING_RIGHT) {
            _velocity.x += _moveSpeed * elapsedTime;
        }
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

    // Only using 2 frames for movement
    std::vector<Frame> walkFrames = {
     {32, 64, 64, 128},   // Frame 1
     {0, 64, 32, 128}     // Frame 2
    };

    // Using the same animation for all states
    _animations[eStatus::NORMAL] = new Animation(spriteSheet, walkFrames, 0.2f);
    _animations[eStatus::MOVING_LEFT] = new Animation(spriteSheet, walkFrames, 0.15f);
    _animations[eStatus::MOVING_RIGHT] = new Animation(spriteSheet, walkFrames, 0.15f);
    _animations[eStatus::ATTACKING] = new Animation(spriteSheet, walkFrames, 0.15f);
    _animations[eStatus::DIE] = new Animation(spriteSheet, walkFrames, 0.15f);
}

void Zombie::DetectPlayer(float playerX, float /*playerY*/) {
   /* float distX = playerX - x;
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
    }*/
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