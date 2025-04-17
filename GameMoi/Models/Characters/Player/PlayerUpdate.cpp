#include "Player.h"


void Player::HandleCollision(float elapsedTime) {
    CollisionManager::GetInstance()->Process(collider, elapsedTime, groundColliders);

    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;

    collider->GetSpeed(_velocity.x, _velocity.y);
}

void Player::HandleStateChange(float elapsedTime) {
    isOnGround = false;
    for (auto& g : groundColliders) {
        float l1, t1, r1, b1;
        collider->GetBoundingBox(l1, t1, r1, b1);
        float l2, t2, r2, b2;
        g->GetBoundingBox(l2, t2, r2, b2);

        const float epsilon = 1.0f;
        float verticalOffset = (state == PlayerState::SitDown || state == PlayerState::Jumping) ? 3.0f : 0.0f;

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

    if (isOnGround && state == PlayerState::Walking)
        x += _velocity.x * elapsedTime;
}

void Player::Update(float elapsedTime) {
    if (!isClimbing)
        _velocity.y += _gravity * elapsedTime + 1.0f;

    collider->vx = _velocity.x;
    collider->vy = _velocity.y;

    HandleCollision(elapsedTime);
    HandleStateChange(elapsedTime);
    HandleWeaponUpdate(elapsedTime);
    animations[state].Update(elapsedTime);
    HandleStairInteraction(elapsedTime);

}