#include "GameObject.h"

void GameObject::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (texture) {
        spriteBatch->Draw(texture, DirectX::XMFLOAT2(x, y));
    }
}
/*
#include "GameObject.h"

GameObject::GameObject(float x, float y, ID3D11ShaderResourceView* texture)
    : x(x), y(y), texture(texture), _velocity(0, 0) {
    _sprite = nullptr;
    _id = eID::UNKNOWN;
    _status = eStatus::NORMAL;
    _physicsSide = eDirection::NONE;
}

GameObject::~GameObject() {
    // Giải phóng tài nguyên nếu có
    if (_sprite) {
        delete _sprite;
        _sprite = nullptr;
    }
}

void GameObject::Update(float elapsedTime) {
    // Cập nhật vị trí dựa trên vận tốc
    x += _velocity.x * elapsedTime;
    y += _velocity.y * elapsedTime;

    // Nếu có sprite, cập nhật luôn
    if (_sprite) {
        _sprite->setPosition(x, y);
    }
}

void GameObject::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (texture) {
        DirectX::XMFLOAT2 pos(x, y);
        spriteBatch->Draw(texture, pos);
    }
}

// ---- Getter & Setter vị trí ----
GVector2 GameObject::getPosition() {
    return GVector2(x, y);
}

void GameObject::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

// ---- Quản lý vận tốc ----
GVector2 GameObject::getVelocity() {
    return _velocity;
}

void GameObject::setVelocity(float vx, float vy) {
    _velocity.x = vx;
    _velocity.y = vy;
}

// ---- Quản lý trạng thái ----
eStatus GameObject::getStatus() {
    return _status;
}

void GameObject::setStatus(eStatus status) {
    _status = status;
}

void GameObject::addStatus(eStatus status) {
    _status = eStatus(_status | status);
}

void GameObject::removeStatus(eStatus status) {
    _status = eStatus(_status & ~status);
}

bool GameObject::isInStatus(eStatus status) {
    return (_status & status) == status;
}

// ---- Xử lý va chạm ----
float GameObject::checkCollision(GameObject* object, float dt) {
    // Đơn giản hóa: nếu bounding box trùng nhau -> có va chạm
    RECT r1 = this->getBounding();
    RECT r2 = object->getBounding();

    if (r1.right > r2.left && r1.left < r2.right &&
        r1.bottom > r2.top && r1.top < r2.bottom) {
        return dt; // Va chạm xảy ra
    }
    return 0.0f; // Không có va chạm
}

RECT GameObject::getBounding() {
    RECT rect;
    rect.left = (LONG)x;
    rect.top = (LONG)y;
    rect.right = (LONG)(x + 32);  // Giả sử kích thước sprite là 32x32
    rect.bottom = (LONG)(y + 32);
    return rect;
}

// ---- Quản lý hướng vật lý ----
void GameObject::setPhysicsBodySide(eDirection side) {
    _physicsSide = side;
}

eDirection GameObject::getPhysicsBodySide() {
    return _physicsSide;
}

*/