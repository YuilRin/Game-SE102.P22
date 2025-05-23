#include "Player.h"
#include "../../World.h"

void Player::ValidateTargetPosition() {
    if (!world) return;

    // Tạo một collider tạm để kiểm tra vị trí target
    float tempL = targetPosition.x - 16; // Half of collider width (32/2)
    float tempT = targetPosition.y - 32; // Half of collider height (64/2) 
    float tempR = targetPosition.x + 16;
    float tempB = targetPosition.y + 32;

    // Kiểm tra xem target position có nằm trên ground nào không
    bool willBeOnGround = false;
    float nearestGroundY = targetPosition.y;

    for (auto& g : world->GetGroundColliders()) {
        float l2, t2, r2, b2;
        g->GetBoundingBox(l2, t2, r2, b2);

        // Kiểm tra overlap theo chiều ngang
        if (tempR > l2 && tempL < r2) {
            // Kiểm tra khoảng cách theo chiều dọc
            float distanceToGround = abs(tempB - t2);

            if (distanceToGround <= 10.0f) { // Trong phạm vi hợp lý
                willBeOnGround = true;
                // Tính toán vị trí y chính xác để đứng trên ground
                float correctY = t2 - 32; // t2 - half collider height
                nearestGroundY = correctY;
                break;
            }
        }
    }

    // Nếu target position sẽ nằm trên ground, điều chỉnh y
    if (willBeOnGround) {
        targetPosition.y = nearestGroundY;
    }
}

void Player::AdjustPositionToNearestGround() {
    if (!world) return;

    float l1, t1, r1, b1;
    collider->GetBoundingBox(l1, t1, r1, b1);

    float closestGroundY = y; // Default to current position
    bool foundGround = false;
    const float searchRange = 50.0f; // Tìm trong phạm vi 50 pixels

    // Tìm ground collider gần nhất trong phạm vi
    for (auto& g : world->GetGroundColliders()) {
        float l2, t2, r2, b2;
        g->GetBoundingBox(l2, t2, r2, b2);

        // Kiểm tra xem có overlap theo chiều ngang không
        if (r1 > l2 && l1 < r2) {
            // Kiểm tra khoảng cách theo chiều dọc
            float distanceToGround = abs(b1 - t2);

            if (distanceToGround <= searchRange) {
                // Tính toán vị trí y để đứng đúng trên ground
                float adjustedY = t2 - (b1 - y); // t2 - collider height

                if (!foundGround || abs(adjustedY - y) < abs(closestGroundY - y)) {
                    closestGroundY = adjustedY;
                    foundGround = true;
                }
            }
        }
    }

    // Nếu tìm thấy ground gần nhất, điều chỉnh vị trí
    if (foundGround) {
        y = closestGroundY;
        collider->SetPosition(x, y);
        _velocity.y = 0; // Reset vertical velocity
        collider->vy = 0;
    }
}

void Player::HandleCollision(float elapsedTime)
{
    if (!world) return;

    auto& ground = world->GetGroundColliders();
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

    // Kiểm tra ground colliders
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

    // Xử lý chuyển đổi state sau khi leo cầu thang
    if ((state == PlayerState::Up || state == PlayerState::Down) &&
        !isSteppingOneStair && !isClimbing && isOnGround) {
        state = PlayerState::Idle;
        _velocity = Vector2(0, 0);
    }

    // Xử lý attack states
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
        x += _velocity.x * elapsedTime;

        const float friction = 0.9f;
        _velocity.x *= friction;

        static float damageTimer = 0.0f;
        damageTimer += elapsedTime;

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

    // Di chuyển ngang
    if (isOnGround && state == PlayerState::Walking && !isClimbing && !isSteppingOneStair)
        x += _velocity.x * elapsedTime;
}

void Player::Update(float elapsedTime) {
    // Xử lý leo cầu thang từng bước
    if (isSteppingOneStair) {
        stepTimer += elapsedTime;

        if (stepTimer >= stepDuration) {
            // Hoàn thành 1 bước
            x = targetPosition.x;
            y = targetPosition.y;
            collider->SetPosition(x, y);

            // Reset trạng thái
            isSteppingOneStair = false;
            stepTimer = 0.0f;
            _velocity = Vector2(0, 0);

            // Cập nhật lại stair interaction sau khi di chuyển xong
            HandleStairInteraction(0.0f);

            // QUAN TRỌNG: Xử lý state transition ngay sau khi hoàn thành step
            if (!isOnStair) {
                // Không còn trên cầu thang - cần kiểm tra và điều chỉnh vị trí
                AdjustPositionToNearestGround();
                state = PlayerState::Idle;
                isClimbing = false;
            }
            else {
                // Vẫn còn trên cầu thang - giữ trạng thái leo nhưng không đang step
                // State sẽ được xử lý trong HandleStateChange
            }
        }
        else {
            // Interpolation từ startPosition đến targetPosition
            float progress = stepTimer / stepDuration;
            // Sử dụng smooth interpolation
            progress = progress * progress * (3.0f - 2.0f * progress); // smoothstep

            x = startPosition.x + (targetPosition.x - startPosition.x) * progress;
            y = startPosition.y + (targetPosition.y - startPosition.y) * progress;
            collider->SetPosition(x, y);
        }

        // Trong khi stepping, vẫn cần update stair interaction để maintain correct blocking states
        HandleStairInteraction(elapsedTime);
    }
    else {
        // Xử lý tương tác với cầu thang khi không đang stepping
        HandleStairInteraction(elapsedTime);
    }

    // Chỉ áp dụng trọng lực khi không đang leo và không đang nhận damage và không đang stepping
    if (!isClimbing && state != PlayerState::TakingDamage && !isSteppingOneStair &&
        state != PlayerState::Up && state != PlayerState::Down) {
        _velocity.y += _gravity * elapsedTime + 1.0f;
    }

    collider->vx = _velocity.x;
    collider->vy = _velocity.y;

    // Chỉ xử lý collision khi không đang stepping để tránh conflict
    if (!isClimbing && state != PlayerState::TakingDamage && !isSteppingOneStair &&
        state != PlayerState::Up && state != PlayerState::Down) {
        HandleCollision(elapsedTime);
    }

    HandleStateChange(elapsedTime);
    HandleWeaponUpdate(elapsedTime);
    animations[state].Update(elapsedTime);
}