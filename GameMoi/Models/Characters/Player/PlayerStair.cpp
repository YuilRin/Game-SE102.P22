#include "Player.h"
#include "../../World.h"
#include <sstream>
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

    // Điều chỉnh khoảng cách bước để phù hợp với tile size
    float adjustedStepDistance = stairStepDistance;

    // Tính toán target position và kiểm tra để tránh "dư" ra khỏi ground
    targetPosition = startPosition + stepDirection * adjustedStepDistance;

    // Kiểm tra và điều chỉnh target position nếu cần
    ValidateTargetPosition();

    isClimbing = true;
    state = PlayerState::Up;
    isSteppingOneStair = true;
    stepTimer = 0.0f;
}

void Player::ClimbDown()
{
    // Kiểm tra xem có thực sự đang trên cầu thang không
    if (!isOnStair || currentStair == nullptr) {
        return;
    }

    // Kiểm tra xem có đang trong quá trình stepping không
    if (isSteppingOneStair) {
        return;
    }

    // Nếu đang ở dưới đất, cần kiểm tra kỹ hơn
    if (isOnGround) {
        // Kiểm tra xem Simon có thực sự đang đứng trên stairTop không
        // bằng cách kiểm tra ground collider mà Simon đang đứng trên
        bool isStandingOnStairTop = false;

        if (world != nullptr) {
            float l1, t1, r1, b1;
            collider->GetBoundingBox(l1, t1, r1, b1);
            for (auto& g : world->GetStairColliders()) {
                float l2, t2, r2, b2;
                g->GetBoundingBox(l2, t2, r2, b2);

                const float epsilon = 2.0f;
                // Kiểm tra xem có đang đứng trên ground này không
                if (abs(b1 - t2) < epsilon && r1 > l2 && l1 < r2) {
                    StairCollider* stairCol = dynamic_cast<StairCollider*>(g);

                    if (stairCol != nullptr && stairCol->IsTop()) {
                        isStandingOnStairTop = true;
                        break;
                    }
                }
            }
        }

        // Nếu đang ở dưới đất nhưng không đứng trên stairTop thì không cho climb down
        if (!isStandingOnStairTop) {
            return;
        }
    }

    // Tính toán hướng di chuyển dựa trên direction của stair
    StairDirection dir = currentStair->GetDirection();
    Vector2 stepDirection = (dir == StairDirection::LeftUp) ? Vector2(-1, 1) : Vector2(1, 1);

    // Bắt đầu leo 1 bước xuống
    startPosition = Vector2(x, y);

    // Điều chỉnh khoảng cách bước để phù hợp với tile size
    float adjustedStepDistance = stairStepDistance;

    targetPosition = startPosition + stepDirection * adjustedStepDistance;

    // Kiểm tra và điều chỉnh target position nếu cần
    ValidateTargetPosition();

    state = PlayerState::Down;
    isSteppingOneStair = true;
    stepTimer = 0.0f;
    isClimbing = true;
}

void Player::SetAllStairTopsBlocking(bool shouldBlock)
{
    for (auto& stair : world->GetStairColliders()) {
        StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
        if (stairCol != nullptr && stairCol->IsTop()) {
            stairCol->SetFrameBlocking(shouldBlock);
        }
    }
}

void Player::HandleStairInteraction(float elapsedTime)
{
    // Reset tất cả stair tops về trạng thái mặc định trước khi xử lý
    for (auto& stair : world->GetStairColliders()) {
        StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
        if (stairCol != nullptr) {
            stairCol->ResetFrameBlocking();
        }
    }

    // Lưu trạng thái cũ để so sánh
    bool wasOnStair = isOnStair;
    StairCollider* previousStair = currentStair;

    if (isSteppingOneStair) {
        return;
    }

    isOnStair = false;
    currentStair = nullptr;

    float l1, t1, r1, b1;
    collider->GetBoundingBox(l1, t1, r1, b1);

    for (auto& stair : world->GetStairColliders()) {
        float l2, t2, r2, b2;
        stair->GetBoundingBox(l2, t2, r2, b2);
        const float epsilon = 2.0f;

        if (r1 > l2 - epsilon && l1 < r2 + epsilon &&
            b1 > t2 - epsilon && t1 < b2 + epsilon) {

            StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
            if (stairCol != nullptr) {
                currentStair = stairCol;
                isOnStair = true;
                break;
            }
        }
    }

    // Nếu vừa rời khỏi cầu thang và đang leo
    if (wasOnStair && !isOnStair &&
        (state == PlayerState::Up || state == PlayerState::Down) &&
        !isSteppingOneStair) {

        // Điều chỉnh vị trí để đảm bảo đứng đúng trên ground
        AdjustPositionToNearestGround();

        // Chuyển về trạng thái bình thường
        state = PlayerState::Idle;
        isClimbing = false;
        _velocity = Vector2(0, 0);

        // Cập nhật collider velocity
        collider->vx = 0;
        collider->vy = 0;
    }
}