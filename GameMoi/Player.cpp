#include "Player.h"

Player::Player(float x, float y,
    const std::vector<ID3D11ShaderResourceView*>& leftTextures,
    const std::vector<ID3D11ShaderResourceView*>& rightTextures,
    float frameTime)
    : GameObject(x, y), leftAnimation(leftTextures, frameTime), rightAnimation(rightTextures, frameTime) {
    speed = 10.0f;
    //fireDirection = 0;
    currentAnimation = &rightAnimation; // M?c ??nh h??ng ph?i
}

void Player::HandleInput(WPARAM key) {
    switch (key) {
    case 'A': case VK_LEFT:
        x -= speed;
        fireDirection = 0;
        currentAnimation = &leftAnimation;
        break;
    case 'D': case VK_RIGHT:
        x += speed;
        fireDirection = 1;
        currentAnimation = &rightAnimation;
        break;
    case 'W': case VK_UP:
        y -= speed;
        fireDirection = 2;
        break;
    case 'S': case VK_DOWN:
        y += speed;
        fireDirection = 3;
        break;
    case 'J':
        //fireballs.push_back({ x, y, fireDirection });
        break;
    }
}

void Player::Update(float elapsedTime) {
    currentAnimation->Update(elapsedTime);

   /* for (auto& fireball : fireballs) {
        switch (fireball.direction) {
        case 0: fireball.x -= 2.0f; break;
        case 1: fireball.x += 2.0f; break;
        case 2: fireball.y -= 2.0f; break;
        case 3: fireball.y += 2.0f; break;
        }
    }*/
}

void Player::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (currentAnimation) {
        currentAnimation->Render(spriteBatch, x, y);
    }
}
