#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <vector>
#include "Animation.h"
#include "Weapon.h"

enum class PlayerState { 
    Idle, Walking, Jumping, SitDown, Stand_Hit,
    Climbing, Attacking, TakingDamage, Dead, PickingUpItem, Falling

};


class Player : public GameObject {
private:
    float speed;
    float velocityY, 
          velocityX;
    float _gravity = 9.8f;
    
    bool facingLeft;

    bool isOnGround, isOnStair, isOnMovingPlatform;
    bool isHolding, isAttacking, isJumping;
    bool _isDead;

    
    int health;
    int lives;
    int whipLevel=1;  

    bool hasCross; ///??

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

  
    PlayerState state;

    std::map<PlayerState, Animation> animations;

    std::vector<Weapon*> weapons;
    Weapon* currentWeapon;

    float attackTimer = 0.0f;
    const float attackDuration = 0.9f;

    int currentStage;
    bool isChangingStage;

public:
 
    Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device);

    void onKeyPressed(WPARAM key);
    void onKeyReleased(WPARAM key);
    void unhookinputevent();

    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    

    

    void MoveLeft();
    void MoveRight();
    void Jump();
    void Fall();
    void SitDown();
    void StandUp();
    void ClimbUp();
    void ClimbDown();

    void Attack();
    void TakeDamage(int damage);
    void Revive();
    void ChangeState(PlayerState newState);
    void CheckCollision(GameObject* object);

    void ChangeWeapon(WeaponType newType);
    void AddWeapon(Weapon* newWeapon);
    void RemoveWeapon(WeaponType type);
    void UseWeapon(WeaponType type);

    void SetStage(int stageID);
    int GetStage();
    bool IsChangingStage();
    void TransitionToNextStage();

    void Reset();
   
	void UpgradeWhip();
  
};

#endif

