#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "../GameObject.h"
#include "../../Animation/Animation.h"
#include "../../Utilities/StopWatch.h"
#include "../../Utilities/define.h"
#include <map>

class Enemy : public GameObject
{
protected:
    int _health;
    int _damage;
    float _moveSpeed;
    bool _isActive;

    eID _enemyType;
    eStatus _status;
    eDirection _direction;

    bool _isBoss;

    // Animations cho các trạng thái khác nhau
    std::map<eStatus, Animation*> _animations;

    // Stopwatch cho cooldown hành động
    StopWatch* _attackCooldown;
    StopWatch* _stateTimer;

public:
    Enemy(float x, float y, ID3D11ShaderResourceView* texture = nullptr);
    virtual ~Enemy();

    // Override từ GameObject
    virtual void Update(float elapsedTime) override;
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    // Enemy specific methods
    virtual void TakeDamage(int amount);
    virtual void SetState(eStatus newState);
    virtual void LoadAnimations(ID3D11Device* device) = 0; // Pure virtual

    // Getters và setters
    bool IsDead() const { return (_status & eStatus::DIE) == eStatus::DIE; }
    bool IsActive() const { return _isActive; }
    void SetActive(bool active) { _isActive = active; }
    bool IsBoss() const { return _isBoss; }
    eID GetEnemyType() const { return _enemyType; }
    eStatus GetStatus() const { return _status; }
    int GetHealth() const { return _health; }
    int GetDamage() const { return _damage; }
};

#endif // __ENEMY_H__