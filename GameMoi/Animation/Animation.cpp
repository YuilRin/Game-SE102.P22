#include "Animation.h"
#include <SpriteBatch.h>


Animation::Animation() : spriteSheet(nullptr), currentFrame(0), frameTime(0), elapsedTime(0) {}


Animation::Animation(ID3D11ShaderResourceView* spriteSheet, 
    const std::vector<Frame>& frames, float frameTime)
    : spriteSheet(spriteSheet), frames(frames), 
    frameTime(frameTime), elapsedTime(0), currentFrame(0) {
}

void Animation::Update(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        currentFrame = (currentFrame + 1) % frames.size();
        elapsedTime = 0;
    }
}

void Animation::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y, bool flip) {
    if (!spriteSheet || frames.empty()) return;
    const Frame& frame = frames[currentFrame];
    RECT rect = { frame.left, frame.top, frame.right, frame.bottom };
    DirectX::SpriteEffects effect = flip ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
    spriteBatch->Draw(spriteSheet, DirectX::XMFLOAT2(x, y), &rect, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), 1.0f, effect);
}
bool Animation::IsAnimationFinished() const {
    return (currentFrame == frames.size() - 1) && (elapsedTime >= frameTime * 1.5f);
}
void Animation::reset() {
    currentFrame = 0;
    elapsedTime = 0;
}
