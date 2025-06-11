#include "Object.h"
#include "../World.h" 

void Object::SetWorld(World* w)
{
    world = w;
}

Object::Object(float x, float y, ObjectType type, ID3D11ShaderResourceView* texture)
    : GameObject(x, y), type(type), texture(texture), hasIdleAnim(false), hasActiveAnim(false), scale(1.0f)
{
    int width = 32;
    int height = 32;

    // Set dimensions and source rectangles based on object type
    switch (type) {
    case ObjectType::MOVING_STAIR:
        width = 64;
        height = 14;
        sourceRect = { 96, 0, 160, 14 };
        // Initialize velocity for moving stair
        _velocity = Vector2(50.0f, 0.0f); // Start moving right at 50 pixels/second
        break;
    case ObjectType::BLACK_DOOR:
        width = 20;
        height = 50;
        sourceRect = { 160, 0, 180, 50 };
        break;
    case ObjectType::DOWN_ARROW:
        width = 32;
        height = 32;
        sourceRect = { 180, 0, 212, 32 };
        break;
    case ObjectType::UP_ARROW:
        width = 32;
        height = 32;
        sourceRect = { 180, 32, 212, 64 };
        break;
    case ObjectType::LINE:
        width = 60;
        height = 10;
        sourceRect = { 212, 0, 272, 10 };
        break;
    case ObjectType::RIGHT_ARROW:
        width = 16;
        height = 32;
        sourceRect = { 272, 0, 288, 32 };
        break;
    case ObjectType::DOOR_1:
        width = 16;
        height = 96;
        sourceRect = { 0, 64, 16, 160 };
        break;
    case ObjectType::DOOR_2:
        width = 16;
        height = 96;
        sourceRect = { 128, 64, 144, 160 };
        break;
    case ObjectType::TRIDENT:
        // Start with minimal collider - will be updated by animation frames
        width = 8;
        height = 8;
        sourceRect = { 0, 160, 64, 292 };
        break;
    }

    this->collider = new Collider(x, y, width, height);
    collider->SetOwner(this);
}

void Object::UpdateColliderFromAnimation() {
    if (type == ObjectType::TRIDENT) {
        currentFrame = idleAnim.GetCurrentFrameIndex();

        if (currentFrame >= 0 && currentFrame < static_cast<int>(colliderFrames.size())) {
            RECT frameRect = colliderFrames[currentFrame];

            // Calculate width and height from sprite coordinates
            int newWidth = frameRect.right - frameRect.left;
            int newHeight = frameRect.bottom - frameRect.top;

            // Update collider to match sprite size exactly
            if (newWidth > 0 && newHeight > 0) {
                collider->width = newWidth;
                collider->height = newHeight;
                collider->SetPosition(x, y);
            }
            else {
                // Set minimal collider for empty frames
                collider->width = 1;
                collider->height = 1;
                collider->SetPosition(x, y);
            }
        }
    }
}

void Object::HandleCollision(float elapsedTime)
{
    if (!world) { return; }

    auto& ground = world->GetGroundColliders();
    CollisionManager::GetInstance()->Process(collider, elapsedTime, ground);
    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;
    collider->GetSpeed(_velocity.x, _velocity.y);
}

void Object::Update(float elapsedTime)
{
    // Special handling for MOVING_STAIR
    if (type == ObjectType::MOVING_STAIR) {
        // Check for collision with ground/walls to reverse direction
        if (world) {
            auto& ground = world->GetGroundColliders();

            // Predict next position
            float nextX = x + _velocity.x * elapsedTime;

            // Create a temporary collider at next position to check collision
            Collider tempCollider(nextX, y, collider->width, collider->height);

            bool willCollide = false;
            for (auto& g : ground) {
                float l1, t1, r1, b1;
                float l2, t2, r2, b2;

                tempCollider.GetBoundingBox(l1, t1, r1, b1);
                g->GetBoundingBox(l2, t2, r2, b2);

                // Check horizontal collision
                if (!(l1 >= r2 || r1 <= l2) && !(t1 >= b2 || b1 <= t2)) {
                    willCollide = true;
                    break;
                }
            }

            // Reverse direction if collision detected
            if (willCollide) {
                _velocity.x = -_velocity.x;
            }
        }

        // Update position
        x += _velocity.x * elapsedTime;
        collider->SetPosition(x, y);

        // Update collider velocity to match object velocity
        collider->vx = _velocity.x;
        collider->vy = _velocity.y;
    }

    // Handle other object types
    if (isActivated) {
        // Update active animation if it's been set
        if (hasActiveAnim) {
            activeAnim.Update(elapsedTime);
        }
    }
    else {
        // Update idle animation if it's been set
        if (hasIdleAnim) {
            idleAnim.Update(elapsedTime);
        }
    }

    // Update collider for trident based on animation frame
    if (type == ObjectType::TRIDENT) {
        UpdateColliderFromAnimation();
    }
}

void Object::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (isActivated && hasActiveAnim) {
        // Render active animation
        activeAnim.Render(spriteBatch, x, y, false);
    }
    else if (!isActivated && hasIdleAnim) {
        // Render idle animation
        idleAnim.Render(spriteBatch, x, y, false);
    }
    else {
        // Draw static sprite with scale factor
        spriteBatch->Draw(
            texture,
            DirectX::XMFLOAT2(x, y),
            &sourceRect,
            DirectX::Colors::White,
            0.0f,                                  // Rotation (no rotation)
            DirectX::XMFLOAT2(0, 0),               // Origin (top-left corner)
            scale                                  // Scale factor
        );
    }
}

void Object::OnActivate() {
    if (isActivated) return;
    isActivated = true;

    // Reset active animation if it exists
    if (hasActiveAnim) {
        activeAnim.reset();
    }
}

void Object::MarkForDelete()
{
    if (world) {
        world->RemoveObject(this);
    }
    else {
        isActivated = false;
    }
}