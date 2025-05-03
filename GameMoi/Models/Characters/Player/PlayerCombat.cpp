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

    int a = this->GetInfo()->GetHeart();
    this->GetInfo()->SetLife(a - damage);
    char message[50];
    sprintf_s(message, "HeartNumber = %d", this->GetInfo()->GetHeart());
    MessageBoxA(NULL, message, "Thông báo", MB_OK | MB_ICONINFORMATION);
}