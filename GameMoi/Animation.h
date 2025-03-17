#pragma once
#include <vector>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <SpriteBatch.h>

class Animation {
public:
    Animation(const std::vector<ID3D11ShaderResourceView*>& frames, float frameTime);
    void Update(float deltaTime);
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y);


private:
    std::vector<ID3D11ShaderResourceView*> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;
};
