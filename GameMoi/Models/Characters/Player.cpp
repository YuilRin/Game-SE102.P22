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
    : GameObject(x, y), animations(std::move(anims)), state(PlayerState::Idle), facingLeft(false), velocityY(0), isOnGround(false), device(device)
{
    _info = new Info();
    _info->init();
    _info->SetHeart(50);
    _info->SetLife(3);
    _info->SetScore(0);
    _info->SetPlayerHitPoint(16);
    _info->SetEnemyHitPoint(16);
    _info->ActiveTime();
    _info->SetTime(300);

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
    case 'G': {
        char message[50];
        sprintf_s(message, "Tọa độ nhân vật: X = %.2f, Y = %.2f", x, y);
        MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);
        break;
    }
    }
}

void Player::onKeyReleased(WPARAM key) 
{
    switch (key) {
    case 'A': case VK_LEFT:
    case 'D': case VK_RIGHT:
        velocityX = 0;
        if (isOnGround && state == PlayerState::Walking)
            state = PlayerState::Idle;
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
    velocityX = -150.0f;
    facingLeft = true;
}

void Player::MoveRight() {
    if (state != PlayerState::Walking) {
        state = PlayerState::Walking;
        animations[state].reset();
    }
    velocityX = 150.0f;
    facingLeft = false;
}

void Player::SitDown() {
    if (isOnGround && state != PlayerState::SitDown) {
        state = PlayerState::SitDown;
        animations[state].reset();
        y += 13.0f;
    }
}

void Player::Jump() {
    if (isOnGround) {
        velocityY = -4.0f;
        isOnGround = false;
        state = PlayerState::Jumping;
        animations[state].reset();
    }
}

void Player::SetGroundColliders(const std::vector<Collider>& colliders) {
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

void Player::HandleWeaponUpdate(float elapsedTime) {
    if (!currentWeapon->IsActive())
        return;

    float weaponOffsetX = facingLeft ? -20.0f : 20.0f;

    if (auto whip = dynamic_cast<Whip*>(currentWeapon)) {
        whip->SetPos(x + weaponOffsetX - 2, y,facingLeft);
    }
    else {
        HandleAxeUpdate();
    }

    currentWeapon->Update(elapsedTime);
}

void Player::HandleAxeUpdate() {
    if (auto axe = dynamic_cast<Axe*>(currentWeapon)) {
        if (!axe->IsThrown()) {
            axe->SetPos(x, y - 10,facingLeft);
            axe->ResetVelocity();
            axe->SetThrown(true);
        }
    }
}
void Player::unhookinputevent() {
	// Unhook input event
}
template<typename T>
const T& myClamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

void Player::Update(float elapsedTime) {
    //velocityY += 9.8f * elapsedTime;
    //y += velocityY;
    //x += velocityX * elapsedTime;

    //// Tạo collider của player
    //Collider playerCollider(x, y, 32, 64); // chỉnh lại nếu player có kích thước khác

    //for (const auto& ground : groundColliders) {
    //    if (playerCollider.CheckCollision(ground)) {
    //        y = ground.y - playerCollider.height;  // đưa player đứng lên trên mặt gạch
    //        velocityY = 0;
    //        isOnGround = true;
    //        state = PlayerState::Idle;
    //        velocityX = 0;
    //        break;
    //    }
    //}
    //if (isOnGround) {
    //    // Nếu đang đi thì tiếp tục di chuyển theo chiều ngang
    //    if (state == PlayerState::Walking) {
    //        x += velocityX * elapsedTime;
    //    }

    //    // Nếu đang tấn công thì kiểm tra thời gian tấn công
    //    if (state == PlayerState::Stand_Hit) {
    //        attackTimer += elapsedTime;
    //        if (attackTimer >= attackDuration) {
    //            state = PlayerState::Idle;
    //            currentWeapon->SetActive(false);
    //        }
    //    }

    //    // Cập nhật weapon nếu có
    //    HandleWeaponUpdate(elapsedTime);
    //}

    //// Cập nhật animation theo state hiện tại
    //animations[state].Update(elapsedTime);

    float ySan = (x >= 390) ? 290.0f : 340.0f;

    x = myClamp(x, 0.0f, 750.0f);

    if (!isOnGround) {
        velocityY += 9.8f * elapsedTime;
        y += velocityY;
        x += velocityX * elapsedTime;

        if (y >= ySan) {
            y = ySan;
            velocityY = 0;
            isOnGround = true;
            state = PlayerState::Idle;
            velocityX = 0;
        }
    }
    else {
        if (state == PlayerState::Walking) {
            x += velocityX * elapsedTime;
        }

        if (state == PlayerState::Stand_Hit) {
            attackTimer += elapsedTime;
            if (attackTimer >= attackDuration) {
                state = PlayerState::Idle;
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
    // Nếu vũ khí chưa có trong pool, tạo mới
    if (weaponPool.find(newType) == weaponPool.end()) {
        Weapon* newWeapon = nullptr;

        switch (newType) {
        case WeaponType::WHIP:
            newWeapon = new Whip(x, y, whipLevel, device);
            break;
        case WeaponType::AXE:
            newWeapon = new Axe(x, y, device, facingLeft);
            break;
        case WeaponType::DAGGER:
           // newWeapon = new Dagger(x, y, device, facingLeft);
            break;
        case WeaponType::HOLYWATER:
            //newWeapon = new HolyWater(x, y, device, facingLeft);
            break;
        default:
            break;
        }

        if (newWeapon != nullptr)
            weaponPool[newType] = newWeapon;
    }

    // Gán weapon hiện tại
    currentWeapon = weaponPool[newType];
}


void Player::UpgradeWhip() {
    whipLevel++;
    if (whipLevel > 5)
        whipLevel = 5;

    if (currentWeapon->GetType() == WeaponType::WHIP)
        dynamic_cast<Whip*>(currentWeapon)->SetLevel(whipLevel);
}
