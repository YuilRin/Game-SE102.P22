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
    if (state == PlayerState::TakingDamage) {
        return;
    }

    // Apply damage to player's heart
    _info->SetHeart(_info->GetHeart() - damage);

    // If no knockback was applied, still transition to damage state
    if (state != PlayerState::TakingDamage) {
        state = PlayerState::TakingDamage;
    }

    // Add invincibility frames logic here if needed

    // Check if player is dead
    if (_info->GetHeart() <= 0) {
        state = PlayerState::Dead;
        _isDead = true;
    }
}