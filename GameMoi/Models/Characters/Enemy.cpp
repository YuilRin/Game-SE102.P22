#include "Enemy.h"
#include "../../Utilities/GameTime.h"

Enemy::Enemy(float x, float y, ID3D11ShaderResourceView* texture)
    : GameObject(x, y, texture),
    _health(100),
    _damage(10),
    _moveSpeed(50.0f),
    _isActive(true),
    _enemyType(eID::UNKNOWN),
    _status(eStatus::NORMAL),
    _direction(eDirection::NONE),
    _isBoss(false)
{
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

    SAFE_DELETE(_attackCooldown);
    SAFE_DELETE(_stateTimer);
}

void Enemy::Update(float elapsedTime) {
    if (!_isActive) return;

    if ((_status & eStatus::DIE) == eStatus::DIE) {
        // Nếu animation kết thúc thì deactivate enemy
        auto it = _animations.find(_status);
        if (it != _animations.end() && it->second && it->second->IsAnimationFinished()) {
            _isActive = false;
            return;
        }
    }

    // Update animation hiện tại
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
        // Sau khi animation kết thúc, trở về trạng thái bình thường
        auto it = _animations.find(_status);
        if (it != _animations.end() && it->second && it->second->IsAnimationFinished()) {
            SetState(eStatus::NORMAL);
        }
    }
}

void Enemy::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (!_isActive) return;

    // Render animation hiện tại
    auto it = _animations.find(_status);
    if (it != _animations.end() && it->second) {
        bool flip = (_direction == eDirection::LEFT);
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

    _health -= amount;

    if (_health <= 0) {
        _health = 0;
        SetState(eStatus::DIE);
    }
}

void Enemy::SetState(eStatus newState) {
    if (_status != newState) {
        _status = newState;

        // Reset animation khi đổi trạng thái
        auto it = _animations.find(_status);
        if (it != _animations.end() && it->second) {
            it->second->reset();
        }

        _stateTimer->restart();
    }
}