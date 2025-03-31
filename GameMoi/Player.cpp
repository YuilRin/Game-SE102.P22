#include "Player.h"
#include <Windows.h>
#include <map>
#include <string>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "Whip.h"
#include "Axe.h"

Player::Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device)
    : GameObject(x, y), animations(std::move(anims)), state(PlayerState::Idle), facingLeft(false), velocityY(0), isOnGround(false), device(device)
{
    currentWeapon = new Whip(x, y, whipLevel, device);  
}
void Player::HandleInput(WPARAM key) {
    if (state == PlayerState::Jumping) {
        return; 
    }
    if (key == 'K') { 
        if (currentWeapon->GetType() == WeaponType::WHIP) {
            ChangeWeapon(WeaponType::AXE);
        }
        else {
            ChangeWeapon(WeaponType::WHIP);
        }
    }
    PlayerState prevState = state; // Lưu trạng thái trước đó

    if (key == 'A' || key == VK_LEFT) {
        if (state != PlayerState::Walking) {
            state = PlayerState::Walking;
            animations[state].reset(); // Chỉ reset nếu từ trạng thái khác sang Walking
        }
        velocityX = -25.0f;
        facingLeft = true;
    }
    else if (key == 'D' || key == VK_RIGHT) {
        if (state != PlayerState::Walking) {
            state = PlayerState::Walking;
            animations[state].reset();
        }
        velocityX = 25.0f;
        facingLeft = false;
    }
    else if (key == 'S' || key == VK_DOWN) {
        if (isOnGround && state != PlayerState::SitDown) {
            state = PlayerState::SitDown;
            velocityX = 0;
            y += 13.0f;
        }
    }
    else if (key == 'W' || key == VK_UP) {
        if (isOnGround) {
            velocityY = -4.0f;
            isOnGround = false;
            state = PlayerState::Jumping;
        }
    }
    else if (key == 'G') { // Hiển thị tọa độ
        char message[50];
        sprintf_s(message, "Tọa độ nhân vật: X = %.2f, Y = %.2f", x, y);
        MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);
    }
    else if (key == 'J') {
        if (state != PlayerState::Stand_Hit) {
            state = PlayerState::Stand_Hit;
            animations[state].reset();
            attackTimer = 0.0f;
            currentWeapon->SetActive(true);
            currentWeapon->Attack();
        }
    }

}
float frameTimer = 0.0f;

void Player::Update(float elapsedTime) {
    float ySan = 320;
    if (x >= 390) {
        ySan = 280;
    }

    if (!isOnGround) {
        velocityY += 9.8f * elapsedTime;  // Trọng lực
        y += velocityY;
        x += velocityX * elapsedTime;

        if (y >= ySan) { // Chạm đất
            y = ySan;
            velocityY = 0;
            isOnGround = true;

            if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) {
                state = PlayerState::Walking;
            }
            else if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                state = PlayerState::Walking;
            }
            else {
                state = PlayerState::Idle;
                velocityX = 0;  // Ngừng di chuyển ngay lập tức
            }
        }
    }
    else {
        if (state == PlayerState::SitDown) {
            if (!(GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)) {
                state = PlayerState::Idle;
                y -= 13.0f;  // Trả lại y khi nhân vật đứng dậy
            }
        }
        if (currentWeapon->IsActive()) {
            float weaponOffsetX = facingLeft ? -20.0f : 20.0f;

            Whip* whip = dynamic_cast<Whip*>(currentWeapon);
            if (whip) {  // Kiểm tra nếu currentWeapon là Whip
                whip->SetPos(x + weaponOffsetX - 2, y);
            }
            else {
                Axe* axe = dynamic_cast<Axe*>(currentWeapon);
                if (axe && !axe->IsThrown()) {  // Nếu Axe chưa được ném
                    axe->SetPos(this->x, this->y - 10);  // Đặt lại vị trí gần nhân vật
                    axe->ResetVelocity();  // Reset vận tốc
                    axe->SetThrown(true);  // Đánh dấu là đã ném
                }
            }

            currentWeapon->Update(elapsedTime);
        }

        if (state == PlayerState::Stand_Hit) {
            attackTimer += elapsedTime;
            if (attackTimer >= attackDuration) {
                state = PlayerState::Idle;  // Chuyển về Idle khi hết thời gian tấn công
                currentWeapon->SetActive(false);
            }
        }



        if (state == PlayerState::Walking) {
            x += velocityX * elapsedTime;
            animations[state].Update(elapsedTime);

            // ❗ KIỂM TRA NẾU KHÔNG CÒN BẤM PHÍM, CHO NHÂN VẬT NGỪNG NGAY
            if (!(GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000 ||
                GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
                state = PlayerState::Idle;
                velocityX = 0; // Dừng lại ngay
            }
        }
    }

    animations[state].Update(elapsedTime);
	if (currentWeapon->IsActive())
		currentWeapon->Update(elapsedTime); 
}


void Player::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    animations[state].Render(spriteBatch, x, y, facingLeft);
    currentWeapon->Render(spriteBatch);
}
void Player::ChangeWeapon(WeaponType newType) {
    delete currentWeapon;

    switch (newType) {
    case WeaponType::WHIP:
        currentWeapon = new Whip(x, y, whipLevel, device);  // Truyền device vào
        break;
    case WeaponType::DAGGER:
       // currentWeapon = new Dagger(x, y, facingLeft, device);
        break;
    case WeaponType::AXE:
        currentWeapon = new Axe(x, y, device, facingLeft);
        break;
    }
}
void Player::UpgradeWhip() {
	whipLevel++;
	if (whipLevel > 5) {
		whipLevel = 5;
	}
	if (currentWeapon->GetType() == WeaponType::WHIP) {
		dynamic_cast<Whip*>(currentWeapon)->SetLevel(whipLevel);
	}
}

void Player::Attack() {
    if (currentWeapon->IsActive() == false) {
        currentWeapon->Attack();
    }
}

