#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "Animation.h"
#include <vector>/*
class Enemy : public GameObject {
private:
    DirectX::XMFLOAT2 position;
    float speed;
    int dirX, dirY;
    float lastMoveTime;
    Animation leftAnimation;
    Animation rightAnimation;
    Animation* currentAnimation;

public:
    Enemy(float x, float y, const std::vector<ID3D11ShaderResourceView*>& leftTextures,
        const std::vector<ID3D11ShaderResourceView*>& rightTextures, float frameTime);

    void Update(float elapsedTime);
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);
};

*/
#endif
