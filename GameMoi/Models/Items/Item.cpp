#include "Item.h"
#include <DirectXMath.h>
#include "../World.h"



void Item::SetWorld(World* w)
{
    world = w;
}

Item::Item(float x, float y, ItemType type, ID3D11ShaderResourceView* texture)
    : GameObject(x, y), type(type), texture(texture)
{
    this->collider = new Collider(x, y, 16, 16); // hoặc thay đổi size tùy theo item

    switch (type) {
    case ItemType::SMALL_HEART:  sourceRect = { 0, 0, 16, 16 }; break;
    case ItemType::BIG_HEART:    sourceRect = { 16, 0, 40, 20 }; break;
    case ItemType::POT_ROAST:    sourceRect = { 62, 32, 94, 58 }; break;
    case ItemType::AXE:          sourceRect = { 256, 0, 286, 28 }; break;
    default:                     sourceRect = { 0, 0, 16, 16 }; break;
    }
}

void Item::HandleCollision(float elapsedTime)
{
    if (!world)
    {
        return;
    }

    auto& ground = world->GetGroundColliders(); // lấy trực tiếp
    CollisionManager::GetInstance()->Process(collider, elapsedTime, ground);

    float newX, newY;
    collider->GetPosition(newX, newY);
    x = newX;
    y = newY;

    collider->GetSpeed(_velocity.x, _velocity.y);
}


void Item::Update(float dt) {
   
    _velocity.y += 9.8f * dt + 1.0f;

    collider->vx = _velocity.x;
    collider->vy = _velocity.y;
    lifetime -= dt;

    HandleCollision(dt);
    
}

void Item::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (!texture) return;

    spriteBatch->Draw(
        texture,
        DirectX::XMFLOAT2(x, y),
        &sourceRect,
        DirectX::Colors::White
    );
}

void Item::MarkForDelete() {
    // tuỳ bạn triển khai hệ thống xoá object
}
