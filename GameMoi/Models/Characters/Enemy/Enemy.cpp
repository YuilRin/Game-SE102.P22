#include "Enemy.h"
#include "../../../Utilities/GameTime.h"
#include "../../World.h"

void Enemy::SetWorld(World* w)
{
    world = w;
}

void Enemy::HandleCollision(float elapsedTime)
{
    if (!world) return;

    auto& ground = world->GetGroundColliders(); // Get directly
    CollisionManager::GetInstance()->Process(collider, elapsedTime, ground);

    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;

    collider->GetSpeed(_velocity.x, _velocity.y);
}

Enemy::Enemy(float x, float y, ID3D11ShaderResourceView* texture)
    : GameObject(x, y, texture),
    _moveSpeed(75.0f),
    _isActive(true),
    _enemyType(eID::UNKNOWN),
    _status(eStatus::NORMAL),
    _direction(eDirection::NONE),
    _isBoss(false),
    _player(nullptr),
    collider(nullptr),
    world(nullptr)
{
    // Initialize _info in the base class constructor
    _info = new Info();
    _info->init();

    _attackCooldown = new StopWatch();
    _stateTimer = new StopWatch();
}

Enemy::~Enemy() {
    // Clean up animations
    for (auto& pair : _animations) {
        if (pair.second) {
            delete pair.second;
            pair.second = nullptr;
        }
    }
    _animations.clear();

    // Clean up other allocated resources
    SAFE_DELETE(_info);
    SAFE_DELETE(collider);
    SAFE_DELETE(_attackCooldown);
    SAFE_DELETE(_stateTimer);
}

void Enemy::Update(float elapsedTime) {
    if (!_isActive) return;

    // Check if the enemy is dead
    if ((_status & eStatus::DIE) == eStatus::DIE) {
        // If animation is finished, deactivate the enemy
        auto it = _animations.find(_status);
        if (it != _animations.end() && it->second && it->second->IsAnimationFinished()) {
            _isActive = false;
            return;
        }
    }

    // Update current animation
    auto it = _animations.find(_status);
    if (it != _animations.end() && it->second) {
        it->second->Update(elapsedTime);
    }

    // State-specific behavior
    if ((_status & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT) {
        x -= _moveSpeed * elapsedTime;
        _direction = eDirection::LEFT;
    }
    else if ((_status & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT) {
        x += _moveSpeed * elapsedTime;
        _direction = eDirection::RIGHT;
    }

    if ((_status & eStatus::ATTACKING) == eStatus::ATTACKING) {
        // Return to normal state after animation finishes
        auto it = _animations.find(_status);
        if (it != _animations.end() && it->second && it->second->IsAnimationFinished()) {
            SetState(eStatus::NORMAL);
        }
    }
}

void Enemy::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (!_isActive) return;

    // Render current animation
    auto it = _animations.find(_status);
    if (it != _animations.end() && it->second) {
        bool flip = (_direction == eDirection::RIGHT);
        it->second->Render(spriteBatch, x, y, flip);
    }
    else {
        // Fallback to default GameObject rendering if no animation found
        GameObject::Render(spriteBatch);
    }
}

void Enemy::TakeDamage(int amount) {
    if ((_status & eStatus::DIE) == eStatus::DIE)
        return;

    // Ensure _info is not null before using it
    if (_info) {
        _info->TakeDamage(amount);

        if (_info->GetHeart() <= 0) {
            SetState(eStatus::DIE);

        }
    }
}

void Enemy::SetState(eStatus newState) {
    if (_status != newState) {
        _status = newState;

        // Reset animation when changing state
        auto it = _animations.find(_status);
        if (it != _animations.end() && it->second) {
            it->second->reset();
        }

        _stateTimer->restart();
    }
}   