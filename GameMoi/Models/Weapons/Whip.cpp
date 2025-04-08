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
        std::wstring errorMessage = L"Không thể load ảnh roi!\nLỗi: " + std::wstring(errMsg)+filePath.c_str();
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
	  { -23.0f, 15.0f },   // Frame 0: Vị trí tay cầm roi
	  { -45.0f, 0.0f },  // Frame 1: Vị trí tay cầm roi
	  { 19.0f, 15.0f }, // Frame 2: Vị trí tay cầm roi


	  { 30.0f, 15.0f }, // Frame 3: Vị trí tay cầm roi
	  { -10.0f, 5.0f }, // Frame 4: Vị trí tay cầm roi
	  { -20.0f, 10.0f },// Frame 5: Vị trí tay cầm roi

	  { -30.0f, 15.0f },// Frame 6: Vị trí tay cầm roi
	  { -5.0f, -5.0f }, // Frame 7: Vị trí tay cầm roi
	  { -15.0f, 5.0f }, // Frame 8: Vị trí tay cầm roi

	  { -25.0f, 10.0f },// Frame 9: Vị trí tay cầm roi
	  { -35.0f, 15.0f },// Frame 10: Vị trí tay cầm roi
	  { -20.0f, -5.0f },// Frame 11: Vị trí tay cầm roi

	  { -30.0f, 5.0f }, // Frame 12: Vị trí tay cầm roi
	  { -40.0f, 10.0f },// Frame 13: Vị trí tay cầm roi
	  { -50.0f, 15.0f },// Frame 14: Vị trí tay cầm roi

	  { -10.0f, -10.0f}, // Frame 15: Vị trí tay cầm roi
	  { -20.0f, -5.0f}, // Frame 16: Vị trí tay cầm roi
	  { -30.0f, 5.0f},   // Frame 17: Vị trí tay cầm roi
	  
    };
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
    if (isActive) 
    {
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
        int frameIndex = whipAnimation.GetCurrentFrameIndex(); // Lấy index của frame hiện tại
        float offsetX = frameOffsets[frameIndex].first;
        float offsetY = frameOffsets[frameIndex].second;

        
        whipAnimation.Render(spriteBatch, x + offsetX, y + offsetY, facingLeft);
    }
}

void Whip::Attack() {
    isActive = true;
    whipTimer = 0.0f;
    whipAnimation.reset();
}
void Whip::UpdateHitbox() {
	// Cập nhật hitbox dựa trên vị trí và kích thước của roi
	// Ví dụ: nếu roi có kích thước 16x48, bạn có thể cập nhật hitbox như sau:
	//hitbox.x = x;
	//hitbox.y = y;
	//hitbox.width = 16;  // Chiều rộng của roi
	//hitbox.height = 48; // Chiều cao của roi
}
void Whip::Release()
{
    if (texture)
    {
        texture->Release();
        texture = nullptr;
    }

    frameOffsets.clear();
}

