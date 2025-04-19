#include "Player.h"
#include <Windows.h>
#include <map>
#include <string>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "../../Weapons/Whip.h"
#include "../../Weapons/Axe.h"
#include <algorithm> 

const float stairStepX = 250.f; // hoặc tileSize * 0.5 nếu muốn mượt
const float stairStepY = 250.f; // giống trên
bool stand=false;

Player::Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device)
    : GameObject(x, y), animations(std::move(anims)), state(PlayerState::Idle), facingLeft(false), device(device)
{
    _velocity = { 0.0f, 0.0f };
    collider = new Collider(x, y, 32, 64); // kích thước 32x64

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
    switch (key) {
    case 'A': case VK_LEFT:
        if(isOnGround)
            MoveLeft();
        break;
    case 'D': case VK_RIGHT:
        if (isOnGround)
            MoveRight();
        break;
    case 'L': //case VK_DOWN:
        if (isOnGround)
            SitDown();
        break;
    case VK_UP: case 'W':
        ClimbUp(); 
        break;
    case VK_DOWN: case 'S':
        ClimbDown();
        break;

    case 'K': //case VK_UP:
        Jump();
        break;
    case 'J':
        Attack();
        break;
    case 'I':
        ChangeWeapon(currentWeapon->GetType() == WeaponType::WHIP ? WeaponType::AXE : WeaponType::WHIP);
        break;
    case 'G':
    {
        std::string msg = "Ground count: " + std::to_string(groundColliders.size());
        MessageBoxA(nullptr, msg.c_str(), "Debug", MB_OK);
      /*  char message[50];
        sprintf_s(message, "Tọa độ nhân vật: X = %.2f, Y = %.2f", x, y);
        MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);*/
        break;
    }
    case 'O':
        UpgradeWhip();
        break;
    }
}

void Player::onKeyReleased(WPARAM key) {
    switch (key) {
    case 'A': case VK_LEFT:
        _velocity.x = 0;
        state = PlayerState::Idle;
        break;
    case 'D': case VK_RIGHT:
        state = PlayerState::Idle;
        _velocity.x = 0;
        break;
    case VK_UP: case 'W':
        _velocity = Vector2(0, 0);
        if (isOnGround)
        {
            isClimbing = false;
            state = PlayerState::Idle;
        }
        break;
    case VK_DOWN:     case 'S':
        _velocity = Vector2(0, 0);
        if (isOnGround)
        {
            isClimbing = false;
            state = PlayerState::Idle;
        }        
        break;
    case 'L':// case VK_DOWN:
        if (state == PlayerState::SitDown) {
            y -= 13.0f;
          
        }
        break;
    }
   
}

void Player::SetGroundColliders(std::vector<Collider*> colliders) {
    groundColliders = colliders;
}

void Player::SetStairColliders(std::vector<Collider*> colliders)
{
    stairColliders = colliders;
}

void Player::SetItemList(std::vector<Item*>* itemList)
{
    items = itemList;
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

void Player::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    animations[state].Render(spriteBatch, x, y, facingLeft);
    currentWeapon->Render(spriteBatch);
    // render items
    for (auto item : *items) {
        if (item != nullptr) {
            item->Render(spriteBatch);
        }
    
    }

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
