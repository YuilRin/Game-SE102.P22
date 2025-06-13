#pragma once

#include "../Models/GameObject.h"
#include "Collider.h"

// ColliderWrapper để wrap Collider thành GameObject cho QuadTree
class ColliderWrapper : public GameObject {
private:
    Collider* wrappedCollider;

public:
    ColliderWrapper(Collider* collider)
        : GameObject(0, 0, nullptr), wrappedCollider(collider) {
        if (collider) {
            float l, t, r, b;
            collider->GetBoundingBox(l, t, r, b);
            x = l;
            y = t;
            wx = l;
            wy = t;
        }
        _id = eID::COLLIDER_WRAPPER;
        _status = eStatus::NORMAL;
    }

    ~ColliderWrapper() {
        // Không delete wrappedCollider vì nó được quản lý bởi tilemap
        wrappedCollider = nullptr;
    }

    Collider* GetCollider() const { return wrappedCollider; }

    // Override getBounding để trả về đúng bounding box của collider
    RECT getBounding() override {
        if (wrappedCollider) {
            float l, t, r, b;
            wrappedCollider->GetBoundingBox(l, t, r, b);
            return RECT{ (LONG)l, (LONG)t, (LONG)r, (LONG)b };
        }
        return RECT{ 0, 0, 0, 0 };
    }

    // Override Update - ColliderWrapper không cần update logic
    void Update(float elapsedTime) override {
        // ColliderWrapper chỉ là wrapper, không cần update
    }

    // Override Render - ColliderWrapper không cần render
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override {
        // ColliderWrapper không render gì cả
    }

    GameObjectType GetTypeObject() const override {
        return GameObjectType::ITEM; // Hoặc tạo type mới COLLIDER_WRAPPER
    }

    // Override các hàm position để sync với collider
    void GetPosition(float& out_x, float& out_y) override {
        if (wrappedCollider) {
            float l, t, r, b;
            wrappedCollider->GetBoundingBox(l, t, r, b);
            out_x = l;
            out_y = t;
        }
        else {
            out_x = x;
            out_y = y;
        }
    }

    void SetPosition(float newX, float newY) override {
        x = newX;
        y = newY;
        // Collider position thường không thay đổi trong tilemap
    }
};