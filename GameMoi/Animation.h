#pragma once
#include <vector>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <map>
#include <string>



struct Frame {
    int left, top, right, bottom;
};

class Animation {
public:

    Animation(ID3D11ShaderResourceView* spriteSheet, 
        const std::vector<Frame>& frames, 
        float frameTime);
    void Update(float deltaTime);
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y, bool flip);


private:
    
    ID3D11ShaderResourceView* spriteSheet;
    std::vector<Frame> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;
};
