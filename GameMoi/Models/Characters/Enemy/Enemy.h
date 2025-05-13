// Enemy.h
#pragma once

#include "../../GameObject.h"
#include "../../../Animation/Animation.h"
#include "../Info.h"
#include <map>
#include <memory>
#include "../../../Utilities/StopWatch.h"
#include "../../../Tilemap/Collider.h"
#include "../../../Utilities/Vector2.h"

class World;
class Player;

class Enemy : public GameObject {
protected:
    World* world;
    Info* _info;  // The info object for this enemy
    float _moveSpeed;
    bool _isActive;
    eID _enemyType;
    eStatus _status;
    eDirection _direction;
    bool _isBoss;
    Vector2 _velocity;

    Collider* collider;
    Player* _player;  // Reference to player for detection

    StopWatch* _attackCooldown;
    StopWatch* _stateTimer;

    std::map<eStatus, Animation*> _animations;

public:
    Enemy(float x, float y, ID3D11ShaderResourceView* texture);
    virtual ~Enemy();

    virtual void Update(float elapsedTime);
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);

    void SetWorld(World* w);
    void SetPlayer(Player* player) { _player = player; }

    bool IsActive() const { return _isActive; }
    void SetActive(bool active) { _isActive = active; }

    Info* GetInfo() const { return _info; }
    Collider* GetCollider() const { return collider; }

    void SetState(eStatus newState);
    eStatus GetState() const { return _status; }

    void TakeDamage(int amount);

protected:
    void HandleCollision(float elapsedTime);
};