#include "Player.h"

StairCollider* stairTopAsGround = nullptr;

void Player::ClimbUp()
{
    if (!isOnStair || currentStair == nullptr) {
        return;
    }

    if (isSteppingOneStair) {
        return;
    }

    // Tính toán hướng và khoảng cách 1 bước
    StairDirection dir = currentStair->GetDirection();
    Vector2 stepDirection = (dir == StairDirection::LeftUp) ? Vector2(1, -1) : Vector2(-1, -1);

    // Bắt đầu leo 1 bước
    startPosition = Vector2(x, y);
    targetPosition = startPosition + stepDirection * stairStepDistance;

    // Chỉ tắt blocking cho tile hiện tại, không phải tất cả
    if (currentStair->IsTop()) {
        currentStair->SetFrameBlocking(false);
    }

    isClimbing = true;

    // Kiểm tra xem có đang leo lên tile top không
    CheckStairTopTransition(targetPosition);

    state = PlayerState::Up;
    isSteppingOneStair = true;
    stepTimer = 0.0f;
}

void Player::ClimbDown()
{
    if (!isOnStair || currentStair == nullptr) {
        // Kiểm tra xem có đang đứng trên tile top không
        if (stairTopAsGround != nullptr) {
            // Đang đứng trên tile top, cho phép đi xuống
            stairTopAsGround->SetFrameBlocking(false);
            currentStair = stairTopAsGround;
            isOnStair = true;
        }
        else {
            return;
        }
    }

    if (isSteppingOneStair) {
        return;
    }

    // Tính toán hướng và khoảng cách 1 bước xuống
    StairDirection dir = currentStair->GetDirection();
    Vector2 stepDirection = (dir == StairDirection::LeftUp) ? Vector2(-1, 1) : Vector2(1, 1);

    // Bắt đầu leo 1 bước xuống
    startPosition = Vector2(x, y);
    targetPosition = startPosition + stepDirection * stairStepDistance;

    // Chỉ tắt blocking cho tile hiện tại
    if (currentStair->IsTop()) {
        currentStair->SetFrameBlocking(false);
    }

    state = PlayerState::Down;
    isSteppingOneStair = true;
    stepTimer = 0.0f;
    isClimbing = true;
}

void Player::SetAllStairTopsBlocking(bool shouldBlock)
{
    for (auto& stair : stairColliders) {
        StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
        if (stairCol != nullptr && stairCol->IsTop()) {
            stairCol->SetFrameBlocking(shouldBlock);
        }
    }
}

void Player::CheckStairTopTransition(const Vector2& nextPos)
{
    for (auto& stair : stairColliders) {
        StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
        if (stairCol != nullptr && stairCol->IsTop()) {
            float l2, t2, r2, b2;
            stair->GetBoundingBox(l2, t2, r2, b2);

            // Expand the check area slightly to avoid edge cases
            const float tolerance = 2.0f;

            // Kiểm tra xem nextPos có nằm trong tile top không
            if (nextPos.x >= (l2 - tolerance) && nextPos.x <= (r2 + tolerance) &&
                nextPos.y >= (t2 - tolerance) && nextPos.y <= (b2 + tolerance)) {

                // Chỉ tắt blocking nếu đang leo lên tile này
                stairCol->SetFrameBlocking(false);
                break;
            }
        }
    }
}

void Player::HandleStairInteraction(float elapsedTime)
{
    // Reset tất cả stair tops về trạng thái mặc định trước khi xử lý
    for (auto& stair : stairColliders) {
        StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
        if (stairCol != nullptr) {
            stairCol->ResetFrameBlocking();
        }
    }

    if (isSteppingOneStair) {
        // Đang leo từng bước, vẫn cần kiểm tra stair collision
        // nhưng không thay đổi trạng thái leo
        return;
    }

    isOnStair = false;
    currentStair = nullptr;

    // Kiểm tra xem có còn đứng trên stair top không
    if (stairTopAsGround != nullptr) {
        float l2, t2, r2, b2;
        stairTopAsGround->GetBoundingBox(l2, t2, r2, b2);
        float l1, t1, r1, b1;
        collider->GetBoundingBox(l1, t1, r1, b1);

        const float tolerance = 5.0f; // Tăng tolerance để tránh bị mất connection

        // Kiểm tra overlap với tolerance
        bool stillOnTop = (r1 > (l2 - tolerance) && l1 < (r2 + tolerance) &&
            b1 >(t2 - tolerance) && t1 < (b2 + tolerance));

        if (!stillOnTop) {
            stairTopAsGround = nullptr;
        }
        else {
            // Vẫn đứng trên stair top
            if (!isClimbing && state != PlayerState::Up && state != PlayerState::Down) {
                stairTopAsGround->SetFrameBlocking(true);
            }
            else {
                // Đang leo thì không block
                stairTopAsGround->SetFrameBlocking(false);
            }
        }
    }

    float l1, t1, r1, b1;
    collider->GetBoundingBox(l1, t1, r1, b1);

    for (auto& stair : stairColliders) {
        float l2, t2, r2, b2;
        stair->GetBoundingBox(l2, t2, r2, b2);

        if (r1 > l2 && l1 < r2 && b1 > t2 && t1 < b2) {
            StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
            if (stairCol != nullptr) {
                currentStair = stairCol;
                isOnStair = true;

                // Xử lý tile top
                if (stairCol->IsTop()) {
                    float playerBottomY = b1;
                    float stairTopY = t2;
                    const float groundTolerance = 3.0f;

                    // Kiểm tra xem player có đang đứng trên tile top không
                    bool standingOnTop = (playerBottomY <= stairTopY + groundTolerance &&
                        playerBottomY >= stairTopY - groundTolerance);

                    if (standingOnTop && !isClimbing &&
                        state != PlayerState::Up && state != PlayerState::Down) {

                        stairCol->SetFrameBlocking(true);
                        stairTopAsGround = stairCol;
                        isOnGround = true;

                        // Chuyển về idle nếu đang trong trạng thái leo
                        if (state == PlayerState::Up || state == PlayerState::Down) {
                            state = PlayerState::Idle;
                            isClimbing = false;
                        }
                    }
                    else if (isClimbing || state == PlayerState::Up || state == PlayerState::Down) {
                        // Đang leo thì không block
                        stairCol->SetFrameBlocking(false);
                    }
                }
                break;
            }
        }
    }

    // Nếu không còn trên cầu thang và đang leo
    if (!isOnStair && (state == PlayerState::Up || state == PlayerState::Down) && !isSteppingOneStair) {
        state = PlayerState::Idle;
        isClimbing = false;
        _velocity = Vector2(0, 0);
    }
}