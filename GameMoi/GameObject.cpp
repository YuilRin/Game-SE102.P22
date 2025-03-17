#include "GameObject.h"

void GameObject::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (texture) {
        spriteBatch->Draw(texture, DirectX::XMFLOAT2(x, y));
    }
}
