#include "Animation.h"

Animation::Animation(const std::vector<ID3D11ShaderResourceView*>& frames, float frameTime)
    : frames(frames), frameTime(frameTime), elapsedTime(0), currentFrame(0) {
}

void Animation::Update(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        currentFrame = (currentFrame + 1) % frames.size();
        elapsedTime = 0;
    }
}

void Animation::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y) {
    if (frames.empty() || currentFrame >= frames.size() || frames[currentFrame] == nullptr) {
        return; // Không v? n?u frames r?ng ho?c currentFrame l?i
    }
    if (!frames.empty()) {
        spriteBatch->Draw(frames[currentFrame], DirectX::XMFLOAT2(x, y));
    }
}
