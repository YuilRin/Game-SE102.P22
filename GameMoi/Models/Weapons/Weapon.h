#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "../GameObject.h"

enum class WeaponType { WHIP, DAGGER, AXE , HOLYWATER, CROSS, STOPWATCH};

class Weapon : public GameObject {
protected:
    WeaponType type;
    bool isActive;
	bool facingLeft;

public:
    Weapon(float x, float y, WeaponType type, ID3D11ShaderResourceView* texture);
    virtual ~Weapon() {}

    virtual void Update(float elapsedTime) = 0;  
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }
	bool IsFacingLeft() const { return facingLeft; } // Thêm hàm này để kiểm tra hướng vũ khí
	void SetPos(float x, float y, bool facingLeft) { this->x = x; this->y = y; }

    WeaponType GetType() const { return type; }
    virtual void Attack() = 0;  // Mỗi vũ khí sẽ có cách tấn công khác nhau
};

#endif
