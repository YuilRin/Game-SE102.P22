#pragma once
#include "Player2.h"
#include "Models/Characters/Player/Player.h"

// Player2 kế thừa từ Player với điều khiển riêng biệt

// Implementation
Player2::Player2(string startPos, std::map<PlayerState, Animation> anims, ID3D11Device* device)
    : Player(startPos, anims, device) {
    // Có thể đặt vị trí spawn khác cho Player2
    spawnPosition = Vector2(x + 100, y); // Spawn cách Player1 100 pixel
}

void Player2::onKeyPressed(WPARAM key) {
    // Điều khiển cho Player2 - sử dụng các phím khác
    switch (key) {
    case VK_LEFT: 
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
            MoveLeft();
        break;
    case VK_RIGHT: 
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
            MoveRight();
        break;
    case VK_DOWN: 
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead)
            SitDown();
        break;
    case VK_UP: 
        if (!isSteppingOneStair && state != PlayerState::Dead)
            ClimbUp();
        break;
    case 'J':
        if (!isSteppingOneStair && state != PlayerState::Dead)
            ClimbDown();
        break;
    case VK_DELETE:
        if (!isSteppingOneStair && state != PlayerState::Dead)
            Jump();
        break;
    case 'T':
        if (!isSteppingOneStair && state != PlayerState::Dead)
            Attack();
        break;
    case 'R':
        ChangeWeapon(currentWeapon->GetType() == WeaponType::WHIP ? WeaponType::AXE : WeaponType::WHIP);
        break;
    case 'E':
        UpgradeWhip();
        break;
    case 'M':
        ResetToSpawn();
        break;
    }
}

void Player2::onKeyReleased(WPARAM key) {
    switch (key) {
    case VK_LEFT:
    case VK_RIGHT:
        if (isOnGround && !isSteppingOneStair && state != PlayerState::Dead) {
            _velocity.x = 0;
            state = PlayerState::Idle;
        }
        break;
    case VK_UP:
    case 'J': // Giả sử J là xuống
        _velocity = Vector2(0, 0);
        if (isOnGround && state != PlayerState::Dead) {
            isClimbing = false;
            state = PlayerState::Idle;
        }
        break;
    case VK_DOWN:
        if (state == PlayerState::SitDown && state != PlayerState::Dead) {
            y -= 13.0f; // Ngồi thì trừ đi, nhả thì trả lại
        }
        break;
    }
}


void Player2::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    // Có thể thêm hiệu ứng khác biệt cho Player2 (ví dụ: tint màu)
    animations[state].Render(spriteBatch, x, y, facingLeft);
    currentWeapon->Render(spriteBatch);
}

void Player2::ResetToSpawn() {
    x = spawnPosition.x;
    y = spawnPosition.y;
    worldX = x;
    worldY = y;
    collider->SetPosition(x, y);
    _velocity = Vector2(0, 0);
    state = PlayerState::Idle;
}

void Player2::SetSpawnPosition(float newX, float newY) {
    spawnPosition = Vector2(newX, newY);
}