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
    /*small_heart	0	0	16	16
        big_heart	16	0	40	20
        red_money	40	0	70	30
        yellow_money	70	0	100	30
        blue_money	100	0	130	30
        whip_upgrade	130	0	162	32
        dagger		162	0	194	18
        stopwatch	194	0	224	32
        cross		224	0	256	32
        axe		256	0	286	28
        boomerang	0	32	30	60
        holy_water	30	32	62	64
        pot_roast	62	32	94	58
        smallblue_II	94	32	122	60
        bigblue_II	122	32	164	74
        smallred_II	164	32	192	60
        bigred_II	192	32	234	74
        ball_1		234	32	267	60
        ball_2		267	32	295	64
        potion		286	0	312	32
        box_yellow	312	0	344	24
        box_red		344	0	376	24
        crown_yellow	286	32	316	64
        crown_red	316	32	346	64*/
    this->collider = new Collider(x, y, 16, 16); // hoặc thay đổi size tùy theo item

    switch (type) {
    case ItemType::SMALL_HEART:   sourceRect = { 0, 0, 16, 16 }; break;
    case ItemType::BIG_HEART:     sourceRect = { 16, 0, 40, 20 }; break;
    case ItemType::POT_ROAST:     sourceRect = { 62, 32, 94, 58 }; break;
    case ItemType::AXE:           sourceRect = { 256, 0, 286, 28 }; break;
    case ItemType::YELLOW_MONEY:  sourceRect = { 70,0,100,30 }; break;

       
    default:                     sourceRect = { 0, 0, 16, 16 }; break;
    }

    collider->SetOwner(this);
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
    lifetime = 0.0f;
}
