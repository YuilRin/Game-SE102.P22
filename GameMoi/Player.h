#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <vector>
#include "Animation.h"
class Player : public GameObject {
private:
    float speed;
    int fireDirection;
    //std::vector<Fireball> fireballs;
    Animation leftAnimation;
    Animation rightAnimation;
    Animation* currentAnimation;

public:
    Player(float x, float y,
        const std::vector<ID3D11ShaderResourceView*>& leftTextures,
        const std::vector<ID3D11ShaderResourceView*>& rightTextures,
        float frameTime);

    void HandleInput(WPARAM key);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
};

#endif


/*
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Animation.h"
#include "CollisionBody.h"
#include "Weapon.h"
#include "StopWatch.h"
#include <vector>
#include <map>

#define MOVE_SPEED 125
#define JUMP_VEL 350
#define GRAVITY 800
#define ATTACK_TIME 600
#define PROTECT_TIME 2000

class Player : public GameObject {
private:
    float _speed;
    float _gravity;
    bool _isJumping;
    bool _isAttacking;
    bool _isOnGround;
    bool _isDead;

    Animation _leftAnimation;
    Animation _rightAnimation;
    Animation* _currentAnimation;

    std::map<int, Animation*> _animations;
    std::vector<Weapon*> _listWeapon;

    CollisionBody* _collisionBody;
    StopWatch* _attackStopWatch;

public:
    Player(float x, float y,
        const std::vector<ID3D11ShaderResourceView*>& leftTextures,
        const std::vector<ID3D11ShaderResourceView*>& rightTextures,
        float frameTime);

    void HandleInput(WPARAM key);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    // Điều khiển nhân vật
    void moveLeft();
    void moveRight();
    void jump();
    void attack();
    void die();
    void revive();

    // Xử lý va chạm
    float checkCollision(GameObject* object, float dt);
    void applyGravity(float dt);

    bool isAttacking() const;
    bool isDead() const;
};

#endif
*/
