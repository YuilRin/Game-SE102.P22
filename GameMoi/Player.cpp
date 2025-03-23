#include "Player.h"

Player::Player(float x, float y, ID3D11ShaderResourceView* spriteSheet, const std::vector<Frame>& frames, float frameTime)
    : GameObject(x, y), animation(spriteSheet, frames, frameTime) {
}

void Player::HandleInput(WPARAM key) {
    if (key == 'A' || key == VK_LEFT) {
        x -= 10.0f;
        facingLeft = true;
    }
    if (key == 'D' || key == VK_RIGHT) {
        x += 10.0f;
        facingLeft = false;
    }
}

void Player::Update(float elapsedTime) {
    if (!isOnGround) {
        velocityY += _gravity * elapsedTime; // Giả lập trọng lực
        y += velocityY;
        if (y >= 320) { // Giới hạn y để dừng lại khi chạm mặt đất
            y = 320;
            velocityY = 0;
            isOnGround = true;
        }
    }
    animation.Update(elapsedTime);
}
void Player::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    animation.Render(spriteBatch, x, y,facingLeft);
}