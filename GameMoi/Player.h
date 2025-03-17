#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <vector>
#include "Animation.h"
class Player : public GameObject {
private:
    float speed;
    int fireDirection;
    //std::vector<Fireball> fireballs;
    Animation leftAnimation;
    Animation rightAnimation;
    Animation* currentAnimation;

public:
    Player(float x, float y,
        const std::vector<ID3D11ShaderResourceView*>& leftTextures,
        const std::vector<ID3D11ShaderResourceView*>& rightTextures,
        float frameTime);

    void HandleInput(WPARAM key);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;
};

#endif
