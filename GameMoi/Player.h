﻿#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <vector>
#include "Animation.h"

enum class PlayerState { Idle, Walking, Jumping, SitDown };

class Player : public GameObject {
private:
    float speed;
    bool facingLeft;
    float velocityY;
	float velocityX;
	bool isHolding;
    float _gravity = 9.8f;
    bool _isJumping;
    bool _isAttacking;
    bool isOnGround;
    bool _isDead;
  
    PlayerState state;

    std::map<PlayerState, Animation> animations;

    //std::map<int, Animation*> _animations;
    //std::vector<Weapon*> _listWeapon;

//    CollisionBody* _collisionBody;
  //  StopWatch* _attackStopWatch;

public:
 
    Player(float x, float y, std::map<PlayerState, Animation> anims);

    void HandleInput(WPARAM key);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    //void moveLeft();
    //void moveRight();
    //void jump();
    //void attack();
    //void die();
    //void revive();

    //// Xử lý va chạm
    //float checkCollision(GameObject* object, float dt);
    //void applyGravity(float dt);

    //bool isAttacking() const;
    //bool isDead() const;
};

#endif

