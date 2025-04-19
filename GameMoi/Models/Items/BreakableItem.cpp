#include "BreakableItem.h"
#include "../World.h" 

BreakableItem::BreakableItem(float x, float y, Animation breakAnim, ItemType item)
    : GameObject(x, y), breakAnim(breakAnim), hiddenItem(item)
{
    this->collider = new Collider(x, y, 32, 32); // ví dụ size
}

void BreakableItem::Update(float elapsedTime) {
    if (!isBroken) return;

    breakAnim.Update(elapsedTime);

    // Khi animation vỡ xong thì spawn item
//  
}

void BreakableItem::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (!isBroken) {
        // Vẽ sprite ban đầu (nếu bạn có)
    }
    else {
        breakAnim.Render(spriteBatch, x, y, false);
    }
}

void BreakableItem::OnHit() {
    if (isBroken) return;
    isBroken = true;
    breakAnim.reset();
}
