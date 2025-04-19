#pragma once
#include "../GameObject.h"
#include "../../Tilemap/Collider.h"
enum class ItemType {
    SMALL_HEART,
    BIG_HEART,
    RED_MONEY,
    YELLOW_MONEY,
    BLUE_MONEY,
    WHIP_UPGRADE,
    DAGGER,
    STOPWATCH,
    CROSS,
    AXE,
    BOOMERANG,
    HOLY_WATER,
    POT_ROAST,
    SMALL_BLUE,
    BIG_BLUE,
    SMALL_RED,
    BIG_RED,
    BALL_1,
    BALL_2,
    POTION,
    BOX_YELLOW,
    BOX_RED,
    CROWN_YELLOW,
    CROWN_RED
};

class Item : public GameObject {
private:
    ItemType type;
    Collider* collider;
    ID3D11ShaderResourceView* texture;
    RECT sourceRect;
    float lifetime = 5.0f; // tự biến mất sau 5s




public:
   // Item(float x, float y, ItemType type); dùng  TextureManager.h
    Item(float x, float y, ItemType type, ID3D11ShaderResourceView* tex);
    //RECT sourceRect;
    //ID3D11ShaderResourceView* texture= nullptr;


    void Update(float dt) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
    ItemType GetType() const { return type; }
    Collider* GetCollider() const { return collider; }

    void MarkForDelete(); // nếu bạn có logic tự xóa
    bool IsExpired() const { return lifetime <= 0; }
};
