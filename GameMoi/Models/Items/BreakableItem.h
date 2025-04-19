#pragma once
#include "../GameObject.h"
#include "../../Tilemap/Collider.h"
#include "../../Animation/Animation.h"
#include "Item.h"

class BreakableItem : public GameObject {
private:
    bool isBroken = false;
    Collider* collider;
    Animation breakAnim;
    ItemType hiddenItem;

public:
    BreakableItem(float x, float y, Animation breakAnim, ItemType item);

    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    void OnHit(); // ← Gọi khi bị vũ khí chạm
    bool IsBroken() const { return isBroken; }
    Collider* GetCollider() const { return collider; }
};
