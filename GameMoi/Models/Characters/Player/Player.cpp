#include "Player.h"
#include <Windows.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "../../Weapons/Whip.h"
#include "../../Weapons/Axe.h"
#include <algorithm> 

const float stairStepX = 8; // hoặc tileSize * 0.5 nếu muốn mượt
const float stairStepY = 8; // giống trên
bool stand=false;


void Player::SetWorld(World* w)
{
    world = w;
}

Player::Player(string startPos, std::map<PlayerState, Animation> anims, ID3D11Device* device)
    : GameObject(x, y), animations(std::move(anims)), state(PlayerState::Idle), facingLeft(false), device(device)
{
#pragma region startPos

    std::ifstream file(startPos);
    std::string dummy;
    std::getline(file, dummy);
    file >> x >> y;
#pragma endregion

    _velocity = { 0.0f, 0.0f };
    collider = new Collider(x, y, 32, 64); // kích thước 32x64
    collider->SetOwner(this);
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

Player::~Player()
{
    delete _info;
}

void Player::onKeyPressed(WPARAM key) {
    switch (key) {
    case 'A': 
        if(isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
            MoveLeft();
        break;
    case 'D': 
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
            MoveRight();
        break;
    case 'L':
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
            SitDown();
        break;
    case 'W':
        if(!isSteppingOneStair && state != PlayerState::Dead)
        ClimbUp(); 
        break;
    case 'S':
       
        if (!isSteppingOneStair && state != PlayerState::Dead)
            ClimbDown();
        
        break;

    case VK_SPACE: //case VK_UP:
        if( !isSteppingOneStair && state != PlayerState::Dead)
        Jump();
        break;
    case 'J':

        if (!isSteppingOneStair && state != PlayerState::Dead)
        Attack();
        break;
    case 'I':
        ChangeWeapon(currentWeapon->GetType() == WeaponType::WHIP ? WeaponType::AXE : WeaponType::WHIP);
        break;
    case 'G':
    {
        char message[50];
        sprintf_s(message, "Tọa độ trong thế giới: X = %.2f, Y = %.2f", worldX, worldY);
        MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);
        break;
    }
    case 'Q':
    {
        char message[50];
        sprintf_s(message, "HeartNumber = %d", this->GetInfo()->GetHeart());
        MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);
    }
    case 'O':
        UpgradeWhip();
        break;
    }
}

void Player::onKeyReleased(WPARAM key) {
    switch (key) {
    case 'A': case VK_LEFT: case 'D': case VK_RIGHT:
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
        {
        _velocity.x = 0;
        state = PlayerState::Idle;
    }
        break;
       
    case VK_UP: case 'W': case VK_DOWN:     case 'S':
        _velocity = Vector2(0, 0);
        if (isOnGround && state != PlayerState::Dead)
        {
            isClimbing = false;
            state = PlayerState::Idle;
        }
        break;
    case 'L':
        if (state == PlayerState::SitDown && state != PlayerState::Dead) {
            y -= 13.0f;
          
        }
        break;
    }
   
}

void Player::SetItemList(std::vector<Item*>* itemList)
{
    //items = itemList;
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
   /* for (auto item : *items) {
        if (item != nullptr) {
            item->Render(spriteBatch);
        }
    
    }*/

}

void Player::ChangeWeapon(WeaponType newType) {
    if (weaponPool.find(newType) == weaponPool.end()) {
        Weapon* newWeapon = nullptr;

        switch (newType) {
        case WeaponType::WHIP:
            newWeapon = new Whip(x, y, whipLevel, device);
            break;
        case WeaponType::AXE:
            //newWeapon = new Axe(x, y, device, facingLeft);
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

int Player::GetStage()
{
    return currentStage;
}
void Player::UpdatePosition(float dt) {
    float nextX = worldX + _velocity.x * dt;
    //float nextY = worldY + _velocity.y * dt;

    // Chặn trái màn hình
    if (nextX < 0) nextX = 0;

    // TODO: Chặn theo tile map nếu có
    // if (IsBlockedAt(nextX, worldY)) nextX = worldX;
    // if (IsBlockedAt(worldX, nextY)) nextY = worldY;

    worldX = nextX;
    //worldY = nextY;

}

