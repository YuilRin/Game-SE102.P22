#include "Player.h"
#include <Windows.h>
#include <map>
#include <string>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "../Weapons/Whip.h"
#include "../Weapons/Axe.h"
#include <algorithm> 

Player::Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device)
    : GameObject(x, y), animations(std::move(anims)), state(PlayerState::Idle), facingLeft(false), device(device)
{
    _velocity = { 0.0f, 0.0f };
    collider = new Collider(x, y, 32, 64); // Ví dụ kích thước 32x64

    isOnGround = false;

    _info = new Info();
    _info->init();
    _info->SetHeart(50);
    _info->SetLife(3);
    _info->SetScore(0);
    _info->SetPlayerHitPoint(16);
    _info->SetEnemyHitPoint(16);
    _info->ActiveTime();
    _info->SetTime(300);

    whipLevel = 1;
    currentWeapon = new Whip(x, y, whipLevel, device);
    
}

void Player::onKeyPressed(WPARAM key) {
    if (state == PlayerState::Jumping)
        return;

    switch (key) {
    case 'A': case VK_LEFT:
        MoveLeft();
        break;
    case 'D': case VK_RIGHT:
        MoveRight();
        break;
    case 'S': case VK_DOWN:
        SitDown();
        break;
    case 'W': case VK_UP:
        Jump();
        break;
    case 'J':
        Attack();
        break;
    case 'K':
        ChangeWeapon(currentWeapon->GetType() == WeaponType::WHIP ? WeaponType::AXE : WeaponType::WHIP);
        break;
    case 'G':
    {
        char message[50];
        sprintf_s(message, "Tọa độ nhân vật: X = %.2f, Y = %.2f", x, y);
        MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);
        break;
    }
    case 'L':
        UpgradeWhip();
        break;
    }
}

void Player::onKeyReleased(WPARAM key) {
    switch (key) {
    case 'A': case VK_LEFT:
        state = PlayerState::Idle;
        _velocity.x = 0;
        break;
    case 'D': case VK_RIGHT:
        state = PlayerState::Idle;
        _velocity.x = 0;
            
        break;
    case 'S': case VK_DOWN:
        if (state == PlayerState::SitDown) {
            y -= 13.0f;
            state = PlayerState::Idle;
        }
        break;
    }
}

void Player::MoveLeft() {
    if (state != PlayerState::Walking) {
        state = PlayerState::Walking;
        animations[state].reset();
    }
    _velocity.x = -150.0f;
    facingLeft = true;
}

void Player::MoveRight() {
    if (state != PlayerState::Walking) {
        state = PlayerState::Walking;
        animations[state].reset();
    }
    _velocity.x = 150.0f;
    facingLeft = false;
}

void Player::SitDown() {
    if (isOnGround && state != PlayerState::SitDown) {
        state = PlayerState::SitDown;
        animations[state].reset();
        
    }
}

void Player::Jump() {
    
    if (isOnGround) {
        _velocity.y = -120.0f;
        isOnGround = false;
        state = PlayerState::Jumping;
        animations[state].reset();
    }
}

void Player::SetGroundColliders(std::vector<Collider*> colliders) {
    groundColliders = colliders;
}

void Player::Attack() {
    if (state == PlayerState::Stand_Hit || currentWeapon->IsActive())
        return;

    state = PlayerState::Stand_Hit;
    animations[state].reset();
    attackTimer = 0.0f;
    currentWeapon->SetActive(true);
    currentWeapon->Attack();
}

void Player::TakeDamage(int damage) {
    // TODO
}

void Player::HandleWeaponUpdate(float elapsedTime) {
    if (!currentWeapon->IsActive())
        return;

    float weaponOffsetX = facingLeft ? -20.0f : 20.0f;

    if (auto whip = dynamic_cast<Whip*>(currentWeapon)) {
        whip->SetPos(x + weaponOffsetX - 2, y, facingLeft);
    }
    else {
        HandleAxeUpdate();
    }

    currentWeapon->Update(elapsedTime);
}

void Player::HandleAxeUpdate() {
    if (auto axe = dynamic_cast<Axe*>(currentWeapon)) {
        if (!axe->IsThrown()) {
            axe->SetPos(x, y - 10, facingLeft);
            axe->ResetVelocity();
            axe->SetThrown(true);
        }
    }
}

void Player::unhookinputevent() {
    // TODO: Unhook input if needed
}

void Player::Update(float elapsedTime) {
    _velocity.y += _gravity * elapsedTime+1.0f;

    collider->vx = _velocity.x;
    collider->vy = _velocity.y;

    CollisionManager::GetInstance()->Process(collider, elapsedTime, groundColliders);

    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;

    collider->GetSpeed(_velocity.x, _velocity.y);

    isOnGround = false;
    for (auto& g : groundColliders) {
        float l1, t1, r1, b1;
        collider->GetBoundingBox(l1, t1, r1, b1);
        float l2, t2, r2, b2;
        g->GetBoundingBox(l2, t2, r2, b2);

        const float epsilon = 1.0f; // hoặc 0.5f
        float verticalOffset = 0.0f;
        if (state == PlayerState::SitDown)
            verticalOffset = 3.0f;
        else if (state == PlayerState::Jumping)
            verticalOffset = 3.0f; // có thể không cần bù

        if (abs((b1 + verticalOffset) - t2) < epsilon && r1 > l2 && l1 < r2) {
            isOnGround = true;
            if (state == PlayerState::Jumping && _velocity.y > 0)
                state = PlayerState::Idle;
            break;
        }
    }

    if (isOnGround) {
        if (state == PlayerState::Walking) {
            x += _velocity.x * elapsedTime;
        }

        if (state == PlayerState::Stand_Hit) {
            attackTimer += elapsedTime;
            if (attackTimer >= attackDuration) {
                state = PlayerState::Idle;
                if (currentWeapon)
                    currentWeapon->SetActive(false);
            }
        }

        HandleWeaponUpdate(elapsedTime);
    }

    animations[state].Update(elapsedTime);
}

void Player::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    animations[state].Render(spriteBatch, x, y, facingLeft);
    currentWeapon->Render(spriteBatch);
}

void Player::ChangeWeapon(WeaponType newType) {
    if (weaponPool.find(newType) == weaponPool.end()) {
        Weapon* newWeapon = nullptr;

        switch (newType) {
        case WeaponType::WHIP:
            newWeapon = new Whip(x, y, whipLevel, device);
            break;
        case WeaponType::AXE:
            newWeapon = new Axe(x, y, device, facingLeft);
            break;
        default:
            break;
        }

        if (newWeapon != nullptr)
            weaponPool[newType] = newWeapon;
    }

    currentWeapon = weaponPool[newType];
}

void Player::UpgradeWhip() {
    whipLevel++;
    if (whipLevel > 5)
        whipLevel = 5;

    if (currentWeapon->GetType() == WeaponType::WHIP)
        dynamic_cast<Whip*>(currentWeapon)->SetLevel(whipLevel);
}
