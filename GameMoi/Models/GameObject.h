#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "../Utilities/define.h"
class GameObject {
protected:
    float x, y;
    eStatus _status;
    ID3D11ShaderResourceView* texture;

public:
    GameObject(float x, float y, ID3D11ShaderResourceView* texture = nullptr)
        : x(x), y(y), texture(texture) {
    }

    virtual ~GameObject() {}
    float GetX();
    float GetY();
    void GetPosition(float x, float& y);
    void SetPosition(float x, float& y);
    virtual void Update(float elapsedTime) = 0;
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);
    RECT GetBounding();
    eStatus getStatus();
    virtual void Release() = 0;
};




#endif
/*
#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
#include "Sprite.h"
#include "define.h"

class GameObject {
protected:
    float x, y;
    GVector2 _velocity; // Vận tốc
    ID3D11ShaderResourceView* texture;
    Sprite* _sprite; // Hỗ trợ Animation
    eID _id; // Định danh đối tượng
    eStatus _status; // Trạng thái hiện tại
    eDirection _physicsSide; // Hướng di chuyển vật lý

public:
    GameObject(float x, float y, ID3D11ShaderResourceView* texture = nullptr)
        : x(x), y(y), texture(texture), _velocity(0, 0) {
        _sprite = nullptr;
        _id = eID::UNKNOWN;
        _status = eStatus::NORMAL;
        _physicsSide = eDirection::NONE;
    }

    virtual ~GameObject() {}

    // Cập nhật và vẽ đối tượng
    virtual void Update(float elapsedTime) = 0;
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);

    // Getter & Setter vị trí
    virtual GVector2 getPosition();
    virtual void setPosition(float x, float y);

    // Quản lý vận tốc
    virtual GVector2 getVelocity();
    virtual void setVelocity(float vx, float vy);

    // Quản lý trạng thái
    virtual eStatus getStatus();
    virtual void setStatus(eStatus status);
    virtual void addStatus(eStatus status);
    virtual void removeStatus(eStatus status);
    virtual bool isInStatus(eStatus status);

    // Quản lý hướng vật lý
    virtual void setPhysicsBodySide(eDirection side);
    virtual eDirection getPhysicsBodySide();

    // Va chạm
    virtual float checkCollision(GameObject* object, float dt);
    virtual RECT getBounding();
};

#endif

*/