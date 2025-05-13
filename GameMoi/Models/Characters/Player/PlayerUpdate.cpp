#include "Player.h"
#include "../../World.h"


void Player::HandleCollision(float elapsedTime)
{
    if (!world) return;

    auto& ground = world->GetGroundColliders(); // lấy trực tiếp
    CollisionManager::GetInstance()->Process(collider, elapsedTime, ground);

    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;

    collider->GetSpeed(_velocity.x, _velocity.y);
}

void Player::HandleStateChange(float elapsedTime) {
    isOnGround = false;

    if (!world) return;

    for (auto& g : world->GetGroundColliders()) {
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

    // Handle TakingDamage state
    if (state == PlayerState::TakingDamage) {
        // Apply velocity from knockback
        x += _velocity.x * elapsedTime;

        // Apply friction to gradually slow down the knockback
        const float friction = 0.9f;
        _velocity.x *= friction;

        // Recovery timer - return to normal state after a short period
        static float damageTimer = 0.0f;
        damageTimer += elapsedTime;

        // After 0.5 seconds, return to idle (adjust timing as needed)
        if (damageTimer >= 0.5f || abs(_velocity.x) < 5.0f) {
            _velocity.x = 0;
            if (isOnGround) {
                state = PlayerState::Idle;
            }
            else {
                state = PlayerState::Falling;
            }
            damageTimer = 0.0f;
        }
    }

    if (isOnGround && state == PlayerState::Walking)
        x += _velocity.x * elapsedTime;
}

void Player::Update(float elapsedTime) {
    if (!isClimbing && state != PlayerState::TakingDamage)
        _velocity.y += _gravity * elapsedTime + 1.0f;

    collider->vx = _velocity.x;
    collider->vy = _velocity.y;

    HandleCollision(elapsedTime);
    HandleStateChange(elapsedTime);
    HandleWeaponUpdate(elapsedTime);
    HandleStairInteraction(elapsedTime);
    animations[state].Update(elapsedTime);
}