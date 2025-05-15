#include "Whip.h"
#include <WICTextureLoader.h>
#include <comdef.h>


Whip::Whip(float x, float y, int level, ID3D11Device* device)
    : Weapon(x, y, WeaponType::WHIP, nullptr), whipLevel(level) {

    isActive = false;
    facingLeft = false;
    whipTimer = 0.0f;
    //whipDuration = 0.9f; // Thời gian roi hoạt động
    std::wstring filePath = L"Image/whip.jpg";
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filePath.c_str(), nullptr, &texture);

    if (FAILED(hr)) {
        // Chuyển mã lỗi HRESULT thành thông báo dễ hiểu
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();

        // Hiển thị thông báo lỗi chi tiết
        if (!device) {
            MessageBoxW(NULL, L"Device is null", L"Lỗi", MB_OK | MB_ICONERROR);
        }
        std::wstring errorMessage = L"Không thể load ảnh roi!\nLỗi: " + std::wstring(errMsg) + filePath.c_str();
        MessageBoxW(NULL, errorMessage.c_str(), L"Lỗi", MB_OK | MB_ICONERROR);
    }

    // Tạo danh sách frame cho từng cấp độ roi
    std::map<int, std::vector<Frame>> whipFrameData = {
        {1, { {0, 0, 16, 48}, {16, 5, 48, 43}, {48, 15, 104, 33} }},
        {5, { {104, 0, 120, 48}, {120, 0, 152, 48}, {208, 15, 296, 33} }},
        {2, { {0, 48, 16, 96}, {16, 48, 48, 96}, {48, 57, 124, 70} }},
        {3, { {0, 96, 16, 144}, {16, 96, 48, 144}, {48, 108, 133, 120} }},
        {4, { {0, 144, 16, 192}, {16, 144, 48, 192}, {48, 160, 133, 172} }}
    };

    whipAnimation = Animation(texture, whipFrameData[level], 0.3f);
    frameOffsets = {
      {{-23.0f, 15.0f}, { 60.0f, 15.0f}},   // Frame 0: Vị trí tay cầm roi
      {{-45.0f,  0.0f}, { 55.0f,  0.0f}},      // Frame 1
      {{ 19.0f, 15.0f}, {-30.0f, 16.0f}},  // Frame 2: Vị trí tay cầm roi	  
    };

    // Tạo collider cho roi, kích thước ban đầu là 0
    collider = new Collider(x, y, 0, 0, 16,48, false);
    collider->SetOwner(this);

    // Thiết lập sát thương theo cấp độ roi
    damage = 1.0f + (level - 1) * 0.5f;
}

void Whip::SetLevel(int level) {
    whipLevel = level;
    //currentFrames = whipFrameData[level];  // lưu frame để dùng sau
    //whipAnimation = Animation(texture, currentFrames, 0.3f);

    // Cập nhật frame dựa trên cấp độ roi mới
    std::map<int, std::vector<Frame>> whipFrameData = {
        {1, { {0, 0, 16, 48}, {16, 0, 48, 48}, {48, 15, 104, 33} }},
        {2, { {104, 0, 120, 48}, {120, 0, 152, 48}, {208, 15, 296, 33} }},
        {3, { {0, 48, 16, 96}, {16, 48, 48, 96}, {48, 57, 124, 70} }},
        {4, { {0, 96, 16, 144}, {16, 96, 48, 144}, {48, 108, 133, 120} }},
        {5, { {0, 144, 16, 192}, {16, 144, 48, 192}, {48, 160, 133, 172} }}
    };

    whipAnimation = Animation(texture, whipFrameData[level], 0.3f);

    // Cập nhật sát thương
    damage = 1.0f + (level - 1) * 0.5f;
}

void Whip::Update(float elapsedTime) {
    if (isActive)
    {
        whipTimer += elapsedTime;
        whipAnimation.Update(elapsedTime);

        // Cập nhật hitbox theo frame hiện tại
        UpdateHitbox();

        if (whipTimer >= whipDuration) {
            isActive = false;
            whipTimer = 0.0f;
        }
    }
}

void Whip::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (isActive) {
        int frameIndex = whipAnimation.GetCurrentFrameIndex(); // Lấy index của frame hiện tại
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        if (facingLeft) {
            offsetX = frameOffsets[frameIndex].second.first;
            if (whipLevel > 1 && frameIndex == 2) {
                offsetX -= 25.0f; // Trừ thêm nếu roi cấp cao hơn
            }
            offsetY = frameOffsets[frameIndex].second.second;
        }
        else {
            offsetX = frameOffsets[frameIndex].first.first;
            offsetY = frameOffsets[frameIndex].first.second;
        }
        whipAnimation.Render(spriteBatch, x + offsetX, y + offsetY, IsFacingLeft());
    }
}

void Whip::Attack() {
    isActive = true;
    whipTimer = 0.0f;
    whipAnimation.reset();
}

void Whip::UpdateHitbox() {
    if (!isActive) {
        // If whip is not active, set collider to 0
        collider->SetPosition(x, y);
        collider->width = 0;
        collider->height = 0;
        return;
    }

    int frameIndex = whipAnimation.GetCurrentFrameIndex();
    float hitboxWidth = 0, hitboxHeight = 0;
    float hitboxX = x, hitboxY = y;

    char whipDebug[200];
    sprintf_s(whipDebug, "Whip active: Timer=%.2f, Duration=%.2f, Frame=%d",
        whipTimer, whipDuration, whipAnimation.GetCurrentFrameIndex());
    //MessageBoxA(NULL, whipDebug, "Whip Status", MB_OK);

    // Only create a hitbox for the extended whip (frame 2)
    if (frameIndex == 2) {
        // Hitbox width increases with whip level
        hitboxWidth = 56 + (whipLevel - 1) * 10;
        hitboxHeight = 15;

        if (facingLeft) {
            hitboxX = x - hitboxWidth + frameOffsets[frameIndex].second.first;
            hitboxY = y + frameOffsets[frameIndex].second.second;
        }
        else {
            hitboxX = x + frameOffsets[frameIndex].first.first;
            hitboxY = y + frameOffsets[frameIndex].first.second;
        }

        // Debug hitbox details
        char hitboxMsg[200];
        sprintf_s(hitboxMsg, "Whip hitbox created: x=%.2f, y=%.2f, w=%.2f, h=%.2f, facing_left=%d",
            hitboxX, hitboxY, hitboxWidth, hitboxHeight, facingLeft);
        //MessageBoxA(NULL, hitboxMsg, "Whip Hitbox", MB_OK);
    }
    else {
        // For other frames, use a very small hitbox or none at all
        hitboxWidth = 0;
        hitboxHeight = 0;

       // MessageBoxA(NULL, "No hitbox for this frame", "Whip Hitbox", MB_OK);
    }

    // Update the collider with new position and dimensions
    collider->SetPosition(hitboxX, hitboxY);
    collider->width = hitboxWidth;
    collider->height = hitboxHeight;
}

void Whip::GetHitboxSize(float& width, float& height) const {
    if (isActive && whipAnimation.GetCurrentFrameIndex() == 2) {
        width = collider->width;
        height = collider->height;
    }
    else {
        width = 0;
        height = 0;
    }
}