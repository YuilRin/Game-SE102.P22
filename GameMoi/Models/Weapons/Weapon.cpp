#include "Weapon.h"

Weapon::Weapon(float x, float y, WeaponType type, ID3D11ShaderResourceView* texture)
    : GameObject(x, y), type(type), isActive(false) {
    this->texture = texture;  // Gán texture từ tham số
}
void Weapon::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (isActive && texture) {
        DirectX::SpriteEffects flipEffect = IsFacingLeft() ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
        spriteBatch->Draw(texture, DirectX::XMFLOAT2(x, y), nullptr, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(20, 0), 1.0f, flipEffect);
    }
}