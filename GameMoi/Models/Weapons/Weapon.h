#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "../GameObject.h"

enum class WeaponType { WHIP, DAGGER, AXE };

class Weapon : public GameObject {
protected:
    WeaponType type;
    bool isActive;

public:
    Weapon(float x, float y, WeaponType type, ID3D11ShaderResourceView* texture);
    virtual ~Weapon() {}

    virtual void Update(float elapsedTime) = 0;  // Phương thức ảo thuần túy
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }
	void SetPos(float x, float y) { this->x = x; this->y = y; }

    WeaponType GetType() const { return type; }
    virtual void Attack() = 0;  // Mỗi vũ khí sẽ có cách tấn công khác nhau
};

#endif
