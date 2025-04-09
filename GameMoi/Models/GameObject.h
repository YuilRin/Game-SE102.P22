#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "../Utilities/define.h" // đường dẫn tương đối tùy dự án
#include "../Tilemap/Collider.h"

class GameObject {
protected:
    float x, y;                             // Vị trí logic
    ID3D11ShaderResourceView* texture;     // Texture vẽ
    Collider* collider = nullptr;          // Va chạm
    GVector2 _velocity;                    // Vận tốc hiện tại
    eID _id;                               // Định danh
    eStatus _status;                       // Trạng thái
    eDirection _physicsSide;              // Hướng vật lý

public:
    GameObject(float x, float y, ID3D11ShaderResourceView* texture = nullptr)
        : x(x), y(y), texture(texture), _velocity(0, 0), _id(eID::UNKNOWN), _status(eStatus::NORMAL), _physicsSide(eDirection::NONE) {
        //collider = new Collider(x, y, 32, 32); // Kích thước mặc định
    }

    virtual ~GameObject() {
        //if (collider) delete collider;
    }

    // Vị trí
    float GetX();
    float GetY();
    virtual void GetPosition(float& out_x, float& out_y);
    virtual void SetPosition(float x, float y);

    // Collider
    //Collider* GetCollider() { return collider; }

    // Vận tốc
    virtual GVector2 getVelocity() { return _velocity; }
    virtual void setVelocity(float vx, float vy) { _velocity = GVector2(vx, vy); }

    // Trạng thái
    virtual eStatus getStatus() { return _status; }
    virtual void setStatus(eStatus status) { _status = status; }
    virtual void addStatus(eStatus status) { _status = static_cast<eStatus>(_status | status); }
    virtual void removeStatus(eStatus status) { _status = static_cast<eStatus>(_status & ~status); }
    virtual bool isInStatus(eStatus status) { return (_status & status) == status; }

    // Hướng vật lý
    virtual void setPhysicsBodySide(eDirection side) { _physicsSide = side; }
    virtual eDirection getPhysicsBodySide() { return _physicsSide; }

    // Va chạm đơn giản (override nếu cần)
    virtual float checkCollision(GameObject* object, float dt) { return 0.0f; }

    // Bounding box (override nếu muốn custom)
    //virtual RECT getBounding() {
    //    float l, t, r, b;
    //    //collider->GetBoundingBox(l, t, r, b);
    //    return RECT{ (LONG)l, (LONG)t, (LONG)r, (LONG)b };
    //}

    // Logic game
    virtual void Update(float elapsedTime) = 0;

    // Vẽ
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);
};

#endif // GAMEOBJECT_H
