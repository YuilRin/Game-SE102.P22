#include "Weapon.h"

Weapon::Weapon(float x, float y, WeaponType type, ID3D11ShaderResourceView* texture)
    : GameObject(x, y), type(type), isActive(false) {
    this->texture = texture;  // Gán texture từ tham số
}

void Weapon::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (isActive && texture) {
        spriteBatch->Draw(texture, DirectX::XMFLOAT2(x, y));
    }
}
