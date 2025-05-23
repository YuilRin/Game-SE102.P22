#pragma once
#include "../GameObject.h"
#include "../../Tilemap/Collider.h"
#include "../../Animation/Animation.h"

class World;

enum class BreakableItemType {
    BIG_CANDLE1,
    BIG_CANDLE,
    CANDLE1,
    CANDLE,
    STAIR,
};

class BreakableItem : public GameObject {
private:
    BreakableItemType type;
    bool isBroken = false;
    bool hasIdleAnim = false;  // Flag to check if idle animation is set
    bool hasBreakAnim = false; // Flag to check if break animation is set
    Collider* collider;
    ID3D11ShaderResourceView* texture;
    RECT sourceRect;
    Animation idleAnim;     // Animation for the idle state
    Animation breakAnim;    // Animation for the broken state
    World* world = nullptr;

public:
    ~BreakableItem() { delete collider; };
    void SetWorld(World* w);
    BreakableItem(float x, float y, BreakableItemType item, ID3D11ShaderResourceView* texture);

    void HandleCollision(float elapsedTime);//ko có trọng lực 1 số cái

    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    BreakableItemType GetType() const { return type; }
    Collider* GetCollider() const { return collider; }

    void OnHit(); // ← Gọi khi bị vũ khí chạm
    bool IsBroken() const { return isBroken; }
    void MarkForDelete(); // Đánh dấu để xóa

    // Set animations directly for the item
    void SetIdleAnimation(const Animation& anim) { idleAnim = anim; hasIdleAnim = true; }
    void SetBreakAnimation(const Animation& anim) { breakAnim = anim; hasBreakAnim = true; }
	BreakableItemType GetBreakableType() const { return type; }

    float scale;
};