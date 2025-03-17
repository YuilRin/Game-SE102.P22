#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteBatch.h"
class GameObject {
protected:
    float x, y;
    ID3D11ShaderResourceView* texture;

public:
    GameObject(float x, float y, ID3D11ShaderResourceView* texture = nullptr)
        : x(x), y(y), texture(texture) {
    }

    virtual ~GameObject() {}

    virtual void Update(float elapsedTime) = 0;
    virtual void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);
};


#endif
