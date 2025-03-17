#include "Enemy.h"
#include <cstdlib>
#include <ctime>
Enemy::Enemy(float x, float y, const std::vector<ID3D11ShaderResourceView*>& leftTextures,
    const std::vector<ID3D11ShaderResourceView*>& rightTextures, float frameTime)
    : GameObject(x, y), leftAnimation(leftTextures, frameTime), rightAnimation(rightTextures, frameTime) {
    speed = 0.1f;
    dirX = 1;
    dirY = 0;
    lastMoveTime = 0.0f;
    currentAnimation = &rightAnimation; // Mặc định hướng phải
}

void Enemy::Update(float elapsedTime) {
    if (elapsedTime - lastMoveTime >= 1.0f) {
        dirX = (rand() % 3) - 1;
        dirY = (rand() % 3) - 1;
        lastMoveTime = elapsedTime;
    }

    x += dirX * speed;
    y += dirY * speed;

    // Chuyển đổi animation
    if (dirX > 0) {
        currentAnimation = &rightAnimation;
    }
    else if (dirX < 0) {
        currentAnimation = &leftAnimation;
    }

    currentAnimation->Update(elapsedTime);
}
void Enemy::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (currentAnimation) {
        currentAnimation->Render(spriteBatch, x, y);
    }
}


