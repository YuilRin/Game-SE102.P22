#pragma once
#include "../GameObject.h"
#include <d3d11.h>
#include <SpriteBatch.h>
#include <memory>
#include "../../Tilemap/Collider.h"

enum class WeaponType {
    WHIP,
    AXE,
    DAGGER,
    HOLY_WATER,
    CROSS,
    NONE
};

class Weapon : public GameObject {
protected:
    WeaponType type;
    bool isActive;
    bool facingLeft;
    float damage;  // Thêm thuộc tính damage
    ID3D11ShaderResourceView* texture;
    Collider* collider; // Thêm collider cho vũ khí

public:
    Weapon(float x, float y, WeaponType type, ID3D11ShaderResourceView* texture);
    virtual ~Weapon() { if (collider) delete collider; }

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);
    virtual void Attack() = 0;

    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }

    void SetFacingLeft(bool left) { facingLeft = left; }
    bool IsFacingLeft() const { return facingLeft; }
   
    void SetPos(float x, float y, bool facingLeft) { this->x = x; this->y = y; }
    WeaponType GetType() const { return type; }

    // Thêm các hàm để lấy và cập nhật collider
    Collider* GetCollider() const { return collider; }
    virtual void UpdateHitbox() = 0; // Hàm ảo để cập nhật hitbox của vũ khí

    // Hàm lấy sát thương
    float GetDamage() const { return damage; }
    void SetDamage(float dmg) { damage = dmg; }
};