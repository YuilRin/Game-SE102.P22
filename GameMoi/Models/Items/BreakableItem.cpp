#include "BreakableItem.h"
#include "../World.h" 

void BreakableItem::SetWorld(World* w)
{
    world = w;
}

BreakableItem::BreakableItem(float x, float y, BreakableItemType type, ID3D11ShaderResourceView* texture)
    : GameObject(x, y), type(type), texture(texture), hasIdleAnim(false), hasBreakAnim(false), scale(1.0f)
{
    int width = 32;
    int height = 64;

    // Điều chỉnh kích thước collider dựa trên loại item
    switch (type) {
    case BreakableItemType::BIG_CANDLE1:
    case BreakableItemType::BIG_CANDLE:
        width = 32;
        height = 64;
        sourceRect = { 0, 0, 32, 64 };
        break;
    case BreakableItemType::CANDLE1:
    case BreakableItemType::CANDLE:
        width = 16;
        height = 32;
        sourceRect = { 64, 0, 80, 32 };
        break;
    case BreakableItemType::STAIR:
        width = 48;
        height = 48;
        sourceRect = { 96, 0, 112, 16 };
        scale = 3.0f; // Scale up STAIR type by 3x
        break;
    }

    this->collider = new Collider(x, y, width, height);
    collider->SetOwner(this);
}

void BreakableItem::HandleCollision(float elapsedTime)
{
    if (!world) { return; }

    auto& ground = world->GetGroundColliders(); // lấy trực tiếp
    CollisionManager::GetInstance()->Process(collider, elapsedTime, ground);
    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;
    collider->GetSpeed(_velocity.x, _velocity.y);
}

void BreakableItem::Update(float elapsedTime)
{
    _velocity.y += 9.8f * elapsedTime + 1.0f;
    collider->vx = _velocity.x;
    collider->vy = _velocity.y;
    HandleCollision(elapsedTime);

    if (isBroken) {
        // Only update break animation if it's been set
        if (hasBreakAnim) {
            breakAnim.Update(elapsedTime);
            if (breakAnim.IsAnimationFinished()) {
                MarkForDelete();
            }
        }
        else {
            // If no break animation, just mark for delete
            MarkForDelete();
        }
    }
    else {
        // Only update idle animation if it's been set
        if (hasIdleAnim) {
            idleAnim.Update(elapsedTime);
        }
    }
}

void BreakableItem::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (!isBroken) {
        // Use idle animation if available, otherwise use source rect
        if (hasIdleAnim) {
            idleAnim.Render(spriteBatch, x, y, false);
        }
        else {
            // Draw with scale factor
            spriteBatch->Draw(
                texture,
                DirectX::XMFLOAT2(x, y),
                &sourceRect,
                DirectX::Colors::White,
                0.0f,                                  // Rotation (no rotation)
                DirectX::XMFLOAT2(0, 0),               // Origin (top-left corner)
                scale                                  // Scale (3x for STAIR, 1x for others)
            );
        }
    }
    else if (hasBreakAnim) {
        // Render break animation only if it exists
        breakAnim.Render(spriteBatch, x, y, false);
    }
}

void BreakableItem::OnHit() {
    if (isBroken) return;
    isBroken = true;
    breakAnim.reset();
}

void BreakableItem::MarkForDelete()
{
    if (world) {
        world->RemoveBreakable(this);
    }
    else {
        isBroken = false;
    }
}