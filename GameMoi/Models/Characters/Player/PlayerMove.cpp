#include "Player.h"

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

void Player::Jump() {

    if (isOnGround) {
        _velocity.y = -120.0f;
        isOnGround = false;
        state = PlayerState::Jumping;
        animations[state].reset();
    }
}

void Player::SitDown() {
    if (isOnGround && state != PlayerState::SitDown) {
        state = PlayerState::SitDown;
        animations[state].reset();

    }
}