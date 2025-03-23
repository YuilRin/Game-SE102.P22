#include "Player.h"

Player::Player(float x, float y, std::map<PlayerState, Animation> anims)
    : GameObject(x, y), animations(std::move(anims)), state(PlayerState::Idle), facingLeft(false), velocityY(0), isOnGround(false) {
  

}void Player::HandleInput(WPARAM key) {
    if (state == PlayerState::Jumping) {
        return; // Nếu đang nhảy, không cho đổi trạng thái
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
}
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

            // Kiểm tra nếu vẫn giữ phím di chuyển thì tiếp tục đi
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
}





void Player::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    animations[state].Render(spriteBatch, x, y, facingLeft);
}
