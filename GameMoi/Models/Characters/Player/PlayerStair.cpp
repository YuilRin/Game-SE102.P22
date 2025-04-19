#include "Player.h"

StairCollider* stairTopAsGround = nullptr;


void Player::ClimbUp()
{
    if (!isOnStair || currentStair == nullptr) {
        state = PlayerState::Idle;
        return;
    }
    StairDirection dir = currentStair->GetDirection();
    stairDirection = (dir == StairDirection::LeftUp) ? Vector2(1, -1) : Vector2(-1, -1);
    state = PlayerState::Climbing;
    _velocity = stairDirection * stairSpeed * 3.125;/// cần chỉnh để đi theo bước
    isClimbing = true;

}

void Player::ClimbDown()
{
    if (!isOnStair || currentStair == nullptr) {
        state = PlayerState::Idle;
        return;
    }
    StairDirection dir = currentStair->GetDirection();
    stairDirection = (dir == StairDirection::LeftUp) ? Vector2(-1, 1) : Vector2(1, 1);
    state = PlayerState::Climbing;
    _velocity = stairDirection * stairSpeed * 3.125;/// cần chỉnh để đi theo bước _velocity = stairDirection.Normalized() * stepLength / elapsedTime;
    isClimbing = true;
}

void Player::HandleStairInteraction(float elapsedTime)
{
    isOnStair = false;    currentStair = nullptr; // reset

    float l1, t1, r1, b1;
    collider->GetBoundingBox(l1, t1, r1, b1);// nhân vật

    for (auto& stair : stairColliders) {
        float l2, t2, r2, b2;
        stair->GetBoundingBox(l2, t2, r2, b2);

        if (r1 > l2 && l1 < r2 && b1 > t2 && t1 < b2)
        {
            StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
            if (stairCol != nullptr)//&& !stairCol->IsTemporarilyDisabled()) 
            {

                bool passedTop = false;
                float stairTopY = t2; // t2 là top của cầu thang top
                float playerBottomY = b1; // b1 là bottom của player

                if (playerBottomY <= stairTopY + 1.0f) {
                    passedTop = true;

                }
                else
                    passedTop = false;
                if (passedTop) {
                    isClimbing = false;
                    float stairTopY = t2; // top của ô top
                    float height = b1 - t1;
                    // y = stairTopY + height ; // +0.5 để không bị float rounding
                    state = PlayerState::Idle;
                    collider->SetPosition(x + 10.0f, y - 10.0f); // cập nhật lại vị trí collider
                    isOnGround = true;
                    // Thêm ô top vào ground
                    stairTopAsGround = stairCol;
                    stairTopAsGround->SetBlocking(true);

                    // 👉 Thêm vào groundColliders và hiện MessageBox ngay lập tức
                    if (std::find(groundColliders.begin(), groundColliders.end(), stairCol) == groundColliders.end()) {
                        groundColliders.push_back(stairCol);

                        // ✅ THÔNG BÁO NGAY KHI THÊM
                        MessageBoxA(nullptr, "✅ Đã thêm ô top vào groundColliders!", "Thông báo", MB_OK | MB_ICONINFORMATION);
                    }


                }


                currentStair = stairCol;
                isOnStair = true;
                break;

            }


        }
    }


    if (!isOnStair && isClimbing) {
        isClimbing = false;
        _velocity = Vector2(0, 0);
    }
}


