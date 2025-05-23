#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "../../GameObject.h"
#include <vector>
#include "../../../Animation/Animation.h"
#include "../../Weapons/Weapon.h"
#include "../Info.h"
#include "../../../Tilemap/Collider.h"
#include "../../../Tilemap/StairCollider.h"
#include "../../../Utilities/Vector2.h"
#include "../../Items/Item.h"

#include <unordered_map>

class World;

enum class PlayerState {
    Back,
    Dead,
    Down,
    Up,
    Be_Hit,
    Idle, Walking, Jumping, SitDown, Stand_Hit, Up_Hit, Down_Hit,
    Climbing, Attacking, TakingDamage, PickingUpItem, Falling,
    Stagte
};

class Player : public GameObject {
private:

    bool isSteppingOneStair = false;
    float stepTimer = 0.0f;
    const float stepDuration = 0.3f; // thời gian hoàn thành 1 bước
    const float stairStepDistance = 25.0f; // khoảng cách 1 bậc thang (tileSize)

    Vector2 targetPosition; // vị trí đích khi leo 1 bước
    Vector2 startPosition;  // vị trí bắt đầu leo 1 bước

    World* world = nullptr;

    float speed;
    float _gravity = 9.8f;
    float stairSpeed = 25.0f;

    Vector2 _velocity = { 0.0f, 0.0f };

    bool facingLeft;

    bool isOnGround, isOnMovingPlatform;
    bool isHolding, isAttacking, isJumping;
    bool _isDead;

    bool isOnStair = false;
    bool isClimbing = false;
    Vector2 stairDirection = Vector2(1, -1); // hoặc (-1, 1) nếu cầu thang ngược

    int health;
    int lives;
    int whipLevel = 1;

    bool hasCross;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

    PlayerState state;
    std::map<PlayerState, Animation> animations;

    std::unordered_map<WeaponType, Weapon*> weaponPool;
    Weapon* currentWeapon;

    float attackTimer = 0.0f;
    const float attackDuration = 0.91f;

    int currentStage;
    bool isChangingStage;

    Info* _info;
    //std::vector<Collider*> stairColliders;
    StairCollider* currentStair = nullptr;

public:
    GameObjectType GetTypeObject() const override { return GameObjectType::PLAYER; }

    void ApplyKnockback(bool fromLeft, float strength);

    void SetWorld(World* w);
    Collider* GetCollider() const { return collider; }

    Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device);
    ~Player();

    //void SetStairColliders(std::vector<Collider*> colliders);
    void SetItemList(std::vector<Item*>* itemList);

    void HandleCollision(float elapsedTime);
    void HandleStateChange(float elapsedTime);

    // Thêm hàm điều chỉnh vị trí
    void AdjustPositionToNearestGround();
    void ValidateTargetPosition();

    void onKeyPressed(WPARAM key);
    void onKeyReleased(WPARAM key);
    void unhookinputevent();

    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    /// Movement
    void MoveLeft();
    void MoveRight();
    void Jump();
    void Fall();
    void SitDown();
    void StandUp();
    void ClimbUp();
    bool IsPositionOnStairTop(float x, float y, float width, float height);
    void ClimbDown();
    void HandleStairInteraction(float elapsedTime);
    void MoveOneStairStep();

    /// Combat
    void Attack();
    void TakeDamage(int damage);
    void Revive();
    void ChangeState(PlayerState newState);
    void CheckCollision(GameObject* object);

    /// Weapon
    void ChangeWeapon(WeaponType newType);
    void AddWeapon(Weapon* newWeapon);
    void RemoveWeapon(WeaponType type);
    void UseWeapon(WeaponType type);
    void HandleWeaponUpdate(float elapsedTime);
    void HandleAxeUpdate();
    Weapon* GetCurrentWeapon() const {
        return currentWeapon;
    }

    /// Stage
    void SetStage(int stageID);
    int  GetStage();
    bool IsChangingStage();
    void TransitionToNextStage();

    void Reset();
    void UpgradeWhip();

    Info* GetInfo() const { return _info; }
    void SetAllStairTopsBlocking(bool shouldBlock);
    bool IsClimbing() const { return isClimbing; }
};

#endif