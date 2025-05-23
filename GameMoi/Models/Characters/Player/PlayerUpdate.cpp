#include "Player.h"
#include "../../World.h"

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

    // Kiểm tra với stair top colliders
    if (!isOnGround) {
        for (auto& stair : stairColliders) {
            StairCollider* stairCol = dynamic_cast<StairCollider*>(stair);
            if (stairCol != nullptr && stairCol->IsTop()) {
                float l1, t1, r1, b1;
                collider->GetBoundingBox(l1, t1, r1, b1);
                float l2, t2, r2, b2;
                stair->GetBoundingBox(l2, t2, r2, b2);

                const float epsilon = 2.0f;

                // Chỉ coi là ground nếu stair top đang blocking
                bool isStairBlocking = stairCol->IsBlocking();

                if (isStairBlocking && abs(b1 - t2) < epsilon && r1 > l2 && l1 < r2) {
                    isOnGround = true;
                    if (state == PlayerState::Jumping && _velocity.y > 0)
                        state = PlayerState::Idle;
                    break;
                }
            }
        }
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

            // Quyết định state tiếp theo dựa trên vị trí hiện tại
            if (isOnStair) {
                // Vẫn trên cầu thang, giữ nguyên state để có thể leo tiếp
                // state đã được set trong ClimbUp/ClimbDown
            }
            else {
                // Không còn trên cầu thang
                state = PlayerState::Idle;
                isClimbing = false;
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
    if (!isSteppingOneStair) {
        HandleCollision(elapsedTime);
    }

    HandleStateChange(elapsedTime);
    HandleWeaponUpdate(elapsedTime);
    animations[state].Update(elapsedTime);
}