#include "Axe.h"
#include <WICTextureLoader.h>
#include <comdef.h>

Axe::Axe(float x, float y, ID3D11Device* device,bool facingLeft)
    : Weapon(x, y, WeaponType::AXE, nullptr), facingLeft(facingLeft) 
{

    // Load ảnh
    std::wstring filePath = L"Image/weapon.png";
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filePath.c_str(), nullptr, &texture);
    
    std::vector<Frame> axeFrames = { {84,	0,	114,	28}, {114,	0,	144,	28}, {144,	0,	174,	28},{174,	0,	204,	28} };
    axeAnimation = Animation(texture, axeFrames, 0.2f);  // Mỗi frame 0.2s

    this->velocityX = facingLeft ? -100.0f : 100.0f;  // Bay ngang theo hướng nhân vật
    this->velocityY = -200.0f;  // Bay lên trước
    this->gravity = 400.0f;
}
void Axe::Update(float elapsedTime) {
    if (!isActive) return;  
    //if (delayTimer < attackDelay) {
    //    delayTimer += elapsedTime;  // Đếm thời gian delay
    //    return;  // Chưa hết thời gian delay, không di chuyển
    //}
    axeAnimation.Update(elapsedTime);
   
    if (isThrown) {  // Chỉ di chuyển nếu đã ném
        this->x += velocityX * elapsedTime;
        this->y += velocityY * elapsedTime;

        // Áp dụng trọng lực
        velocityY += gravity * elapsedTime;

        // Nếu Axe rơi xuống đất, thì biến mất hoặc reset lại
        if (this->y > 360) {  
            this->isActive = false;
            isThrown = false; 
            // Reset trạng thái ném
        }
    }
}
void Axe::ResetVelocity() {
    this->velocityX = facingLeft ? -100.0f : 100.0f;
    this->velocityY = -200.0f;
}
void Axe::SetThrown(bool thrown) { isThrown = thrown; }
bool Axe::IsThrown() { return isThrown; }




void Axe::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (isActive) {
        axeAnimation.Render(spriteBatch, x, y, true);
    }
}

void Axe::Attack() {
    isActive = true;
    axeTimer = 0.0f;
   // delayTimer = 0.0f;
   // ResetVelocity(); 
    axeAnimation.reset();
}
void Axe::Release()
{
    if (texture)
    {
        texture->Release();
        texture = nullptr;
    }

}
