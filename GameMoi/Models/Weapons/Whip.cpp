#include "Whip.h"
#include <WICTextureLoader.h>
#include <comdef.h>


Whip::Whip(float x, float y, int level, ID3D11Device* device)
    : Weapon(x, y, WeaponType::WHIP, nullptr), whipLevel(level) {

    // Load spritesheet roi

    std::wstring filePath = L"Image/whip.png";
    
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filePath.c_str(), nullptr, &texture);

    //HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"whip.png", nullptr, &texture);

    if (FAILED(hr)) {
        // Chuyển mã lỗi HRESULT thành thông báo dễ hiểu
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();

        // Hiển thị thông báo lỗi chi tiết
		if (!device) {
			MessageBoxW(NULL, L"Device is null", L"Lỗi", MB_OK | MB_ICONERROR);
		}
        std::wstring errorMessage = L"Không thể load ảnh roi!\nLỗi: " + std::wstring(errMsg)+filePath.c_str();
        MessageBoxW(NULL, errorMessage.c_str(), L"Lỗi", MB_OK | MB_ICONERROR);
    }

    // Tạo danh sách frame cho từng cấp độ roi
    std::map<int, std::vector<Frame>> whipFrameData = {
        {1, { {0, 0, 16, 48}, {16, 0, 48, 48}, {48, 15, 104, 33} }},
        {5, { {104, 0, 120, 48}, {120, 0, 152, 48}, {208, 15, 296, 33} }},
        {2, { {0, 48, 16, 96}, {16, 48, 48, 96}, {48, 57, 124, 70} }},
        {3, { {0, 96, 16, 144}, {16, 96, 48, 144}, {48, 108, 133, 120} }},
        {4, { {0, 144, 16, 192}, {16, 144, 48, 192}, {48, 160, 133, 172} }}
    };

    whipAnimation = Animation(texture, whipFrameData[level], 0.4f);  // Load animation cấp độ ban đầu
}

void Whip::SetLevel(int level) {
    whipLevel = level;

    // Cập nhật frame dựa trên cấp độ roi mới
    std::map<int, std::vector<Frame>> whipFrameData = {
        {1, { {0, 0, 16, 48}, {16, 0, 48, 48}, {48, 15, 104, 33} }},
        {5, { {104, 0, 120, 48}, {120, 0, 152, 48}, {208, 15, 296, 33} }},
        {2, { {0, 48, 16, 96}, {16, 48, 48, 96}, {48, 57, 124, 70} }},
        {3, { {0, 96, 16, 144}, {16, 96, 48, 144}, {48, 108, 133, 120} }},
        {4, { {0, 144, 16, 192}, {16, 144, 48, 192}, {48, 160, 133, 172} }}
    };

    whipAnimation = Animation(texture, whipFrameData[level], 0.15f);
}
void Whip::Update(float elapsedTime) {
    if (isActive) {
        whipTimer += elapsedTime;
        whipAnimation.Update(elapsedTime);

        if (whipTimer >= whipDuration) {
            isActive = false;
            whipTimer = 0.0f;
        }
    }
}


void Whip::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    if (isActive) {
        whipAnimation.Render(spriteBatch, x, y, false);
    }
}

void Whip::Attack() {
    isActive = true;
    whipAnimation.reset();
}
