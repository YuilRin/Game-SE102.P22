#include "Player.h"

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
    // Check if already in damaged state (to prevent multiple hits at once)
    if (state == PlayerState::TakingDamage || state == PlayerState::Dead) {
        return;
    }

    // Apply damage to player's heart
    _info->SetHeart(_info->GetHeart() - damage);

    // Check if player is dead FIRST
    if (_info->GetHeart() <= 0) {
        state = PlayerState::Dead;
        _isDead = true;
        _velocity = Vector2(0, 0); // Stop all movement when dead
        collider->vx = 0;
        collider->vy = 0;
        return; // Exit immediately when dead
    }

    // Only apply knockback and damage state if not dead
    state = PlayerState::TakingDamage;

    // Add invincibility frames logic here if needed
}

void Player::ApplyKnockback(bool fromLeft, float strength)
{
    // Don't apply knockback if player is dead
    if (state == PlayerState::Dead || _isDead) {
        return;
    }

    float knockbackDirection = fromLeft ? 1.0f : -1.0f;
    _velocity.x = knockbackDirection * strength;

    // Small vertical boost to make the knockback feel more natural
    _velocity.y = -strength * 0.5f;

    // Update collider velocity
    collider->vx = _velocity.x;
    collider->vy = _velocity.y;

    // Only change state to TakingDamage if not already dead
    if (state != PlayerState::Dead) {
        state = PlayerState::TakingDamage;
    }
}