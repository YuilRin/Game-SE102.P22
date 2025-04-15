#include "Player.h"
#include <Windows.h>
#include <map>
#include <string>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "../Weapons/Whip.h"
#include "../Weapons/Axe.h"
#include <algorithm> 

const float stairStepX = 250.f; // hoặc tileSize * 0.5 nếu muốn mượt
const float stairStepY = 250.f; // giống trên
bool stand=false;
StairCollider* stairTopAsGround = nullptr;

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

void Player::ClimbUp()
{
    if (!isOnStair || currentStair == nullptr) {
        state = PlayerState::Idle;
        return;
    }
    StairDirection dir = currentStair->GetDirection();
    stairDirection = (dir == StairDirection::LeftUp) ? Vector2(1, -1) : Vector2(-1, -1);
    state = PlayerState::Climbing;
        _velocity = stairDirection * stairSpeed*3.125;/// cần chỉnh để đi theo bước
    isClimbing = true;
  
}


void Player::ClimbDown()
{
    if (!isOnStair || currentStair == nullptr) {
        state = PlayerState::Idle;
        return;
    }
    StairDirection dir = currentStair->GetDirection();
    stairDirection = (dir == StairDirection::LeftUp) ? Vector2(-1, 1) : Vector2(1, 1);
    state = PlayerState::Climbing;
    _velocity = stairDirection * stairSpeed * 3.125;/// cần chỉnh để đi theo bước _velocity = stairDirection.Normalized() * stepLength / elapsedTime;
    isClimbing = true;
}

void Player::HandleStairInteraction(float elapsedTime)
{
    isOnStair = false;
    currentStair = nullptr; // reset

    float l1, t1, r1, b1;
    collider->GetBoundingBox(l1, t1, r1, b1);// nhân vật

    for (auto& stair : stairColliders) {
        float l2, t2, r2, b2;
        stair->GetBoundingBox(l2, t2, r2, b2);

        if (r1 > l2 && l1 < r2 && b1 > t2 && t1 < b2) 
        {
            
            StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
            if (stairCol != nullptr)//&& !stairCol->IsTemporarilyDisabled()) 
            {
              
                bool passedTop = false;
                float stairTopY = t2; // t2 là top của cầu thang top
                float playerBottomY = b1; // b1 là bottom của player

                if (playerBottomY <= stairTopY + 1.0f) {
                    passedTop = true; 
                 
                }
                if (passedTop) {
                    isClimbing = false; 
                    float stairTopY = t2; // top của ô top
                    float height = b1 - t1;
                   // y = stairTopY + height ; // +0.5 để không bị float rounding
                    state = PlayerState::Idle;
                    collider->SetPosition(x+10.0f, y-10.0f); // cập nhật lại vị trí collider
                    isOnGround = true;
                    // Thêm ô top vào ground
                    stairTopAsGround = stairCol;

                    // 👉 Thêm vào groundColliders và hiện MessageBox ngay lập tức
                    if (std::find(groundColliders.begin(), groundColliders.end(), stairCol) == groundColliders.end()) {
                        groundColliders.push_back(stairCol);

                        // ✅ THÔNG BÁO NGAY KHI THÊM
                        MessageBoxA(nullptr, "✅ Đã thêm ô top vào groundColliders!", "Thông báo", MB_OK | MB_ICONINFORMATION);
                    }


                }
              

                currentStair = stairCol;
                isOnStair = true;
                break;
             
            }

            
        }
    }
    

    if (!isOnStair && isClimbing) {
        isClimbing = false;
        _velocity = Vector2(0, 0);
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

void Player::SetStairColliders(std::vector<Collider*> colliders)
{
    stairColliders = colliders;
}

void Player::Attack() {
    if (currentWeapon->IsActive())
        return;
    currentWeapon->SetFacingLeft(facingLeft);

    if (isOnGround)
        state = PlayerState::Stand_Hit;
    else
        if (_velocity.y < 0)
            state = PlayerState::Up_Hit;
        else
            state = PlayerState::Down_Hit;

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
    if(!isClimbing)
        _velocity.y += _gravity * elapsedTime + 1.0f;

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
    if (state == PlayerState::Stand_Hit
        || state == PlayerState::Up_Hit
        || state == PlayerState::Down_Hit) {
        attackTimer += elapsedTime;
        if (attackTimer >= attackDuration) {
            state = PlayerState::Idle;
            if (currentWeapon)
                currentWeapon->SetActive(false);
        }
    }
    
    if (isOnGround) {
        if (state == PlayerState::Walking) {
            x += _velocity.x * elapsedTime;
        }       
    }

    HandleWeaponUpdate(elapsedTime);
    animations[state].Update(elapsedTime);
    HandleStairInteraction(elapsedTime);

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
