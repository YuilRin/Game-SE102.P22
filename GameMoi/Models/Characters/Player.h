#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "../GameObject.h"
#include <vector>
#include "../../Animation/Animation.h"
#include "../Weapons/Weapon.h"

enum class PlayerState { Idle, Walking, Jumping, SitDown, Stand_Hit };

//enum class PlayerState { Idle, Moving, Jumping, Crouching, Climbing, Attacking, TakingDamage, Dead, PickingUpItem };


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
    int whipLevel=1;  
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

  
    PlayerState state;

    std::map<PlayerState, Animation> animations;
    Weapon* currentWeapon;

    float attackTimer = 0.0f;
    const float attackDuration = 0.9f;

public:
 
    Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device);

    void HandleInput(WPARAM key);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    void ChangeWeapon(WeaponType newType);
	void UpgradeWhip();
    void Attack();

    //void die();
    //void revive();

    //// Xử lý va chạm
    //float checkCollision(GameObject* object, float dt);
    //void applyGravity(float dt);

    //bool isAttacking() const;
    //bool isDead() const;
};

#endif

