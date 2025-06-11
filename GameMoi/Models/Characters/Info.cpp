#include "Info.h"
#include <sstream>
#include <iomanip>
#include "./Player/Player.h"

Info::Info()
{
    _uiTexture = nullptr;
    _fontTexture = nullptr;
	_heartTexture = nullptr;
	_weaponTexture = nullptr;
	//init(); // Khởi tạo các giá trị mặc định
	//_pauseTime = false; // Bắt đầu với thời gian hoạt động
	//_beginTime = 0; // Thay GameTime::getInstance()->getTotalGameTime() nếu cần
}

Info::~Info()
{
}

void Info::init()
{
    _playerHitPointNumber = 16; // Player bắt đầu với full HP
    _enemyHitPointNumber = 0;
    _heartNumber = 16;
    _lifeNumber = 3;
    _scoreNumber = 0;
    _stageNumber = 1;
    _timeNumber = 300;
    _maxEnemyHP = 16;
    _showEnemyBar = false;
    _maxWeapon = 1;
    _pauseTime = false;
    time = 300;
    _uiPosition = { 20.0f, 360.0f }; // Vị trí bắt đầu UI
}

void Info::LoadTextures(ID3D11Device* device)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Image/heal.png", nullptr, &_uiTexture);
    if (FAILED(hr)){
        hr = DirectX::CreateWICTextureFromFile(device, L"heal.png", nullptr, &_uiTexture);
        if (FAILED(hr)){}
    }

    hr = DirectX::CreateWICTextureFromFile(device, L"Image/fontFull.png", nullptr, &_fontTexture);
    if (FAILED(hr)) {
        hr = DirectX::CreateWICTextureFromFile(device, L"fontFull.png", nullptr, &_fontTexture);
        if (FAILED(hr)) {
        }
    }

    hr = DirectX::CreateWICTextureFromFile(device, L"Image/items.png", nullptr, &_heartTexture);
    if (FAILED(hr)) {
        hr = DirectX::CreateWICTextureFromFile(device, L"items.png", nullptr, &_heartTexture);
        if (FAILED(hr)) {
        }
    }
    hr = DirectX::CreateWICTextureFromFile(device, L"Image/weapon.png", nullptr, &_weaponTexture);
    if (FAILED(hr)) {
        hr = DirectX::CreateWICTextureFromFile(device, L"weapon.png", nullptr, &_weaponTexture);
        if (FAILED(hr)) {
        }
    }
}

void Info::update(float deltatime)
{
    if (!_pauseTime)
    {
        static float timeAccumulator = 0.0f;
        timeAccumulator += deltatime;
        if (timeAccumulator >= 1.0f) // 1 second passed
        {
            if (_timeNumber > 0)
            {
                _timeNumber--;
                time = _timeNumber;
            }
            timeAccumulator = 0.0f;
        }
    }
}

void Info::draw(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, ID3D11DeviceContext* context)
{
    if (!spriteBatch || !_uiTexture || !_fontTexture) return;

    float startX = _uiPosition.x;
    float startY = _uiPosition.y;

    // Vẽ Score với kích thước medium (70%)
    DrawText(spriteBatch, "-----", startX, startY, DirectX::Colors::YellowGreen, 2.0f);
    DrawText(spriteBatch, "SCORE", startX, startY, DirectX::Colors::LightGreen, 2.0f);
    DrawText(spriteBatch, FormatNumber(_scoreNumber, 6), startX + 80, startY, DirectX::Colors::Wheat, 1.6f); DrawText(spriteBatch, FormatNumber(_scoreNumber, 6), startX + 80, startY, DirectX::Colors::LightGreen, 1.6f);

    // Vẽ Time với kích thước small (50%)
    DrawText(spriteBatch, "----", startX + 210, startY, DirectX::Colors::YellowGreen, 2.0f);
    DrawText(spriteBatch, "TIME", startX + 210, startY, DirectX::Colors::LightGreen, 2.0f);
    DrawText(spriteBatch, FormatNumber(_timeNumber, 4), startX + 270, startY, DirectX::Colors::Wheat, 1.6f); DrawText(spriteBatch, FormatNumber(_timeNumber, 4), startX + 270, startY, DirectX::Colors::LightGreen, 1.6f);

    // Vẽ Stage với kích thước small

    DrawText(spriteBatch, "-----", startX + 370, startY, DirectX::Colors::YellowGreen, 2.0f);
    DrawText(spriteBatch, "STAGE", startX + 370, startY, DirectX::Colors::LightGreen, 2.0f);
    DrawText(spriteBatch, FormatNumber(_stageNumber, 2), startX + 450, startY, DirectX::Colors::Wheat, 1.6f); DrawText(spriteBatch, FormatNumber(_stageNumber, 2), startX + 450, startY, DirectX::Colors::LightGreen, 1.6f);

    // Vẽ Player health bar
    float playerY = startY + 30;
    DrawText(spriteBatch, "------", startX, playerY, DirectX::Colors::YellowGreen, 2.0f);
    DrawText(spriteBatch, "PLAYER", startX, playerY, DirectX::Colors::LightGreen, 2.0f);
    DrawHealthBar(spriteBatch, startX + 70, playerY + 2, _playerHitPointNumber, 16, true);

    // Vẽ Enemy health bar
    if (_enemyHitPointNumber > 0 || _showEnemyBar)
    {
        float enemyY = playerY + 25;
        DrawText(spriteBatch, "ENEMY", startX, enemyY, DirectX::Colors::LightGreen, 0.6f);
        DrawHealthBar(spriteBatch, startX + 70, enemyY + 2, _enemyHitPointNumber, _maxEnemyHP, false);
    }

    // Vẽ weapon info
    float weaponX = startX + 280;
    DrawWeaponInfo(spriteBatch,startX +40, playerY+30);

    // Vẽ hearts
    DrawHearts(spriteBatch, startX , playerY+30);

    // Vẽ life number với kích thước nhỏ
    DrawText(spriteBatch, FormatNumber(_lifeNumber, 2), startX + 80, playerY, DirectX::Colors::Wheat, 1.6f);
    DrawText(spriteBatch, FormatNumber(_lifeNumber, 2), startX + 80, playerY, DirectX::Colors::Wheat, 1.6f);
}

void Info::DrawWeaponInfo(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y)
{
    // Vẽ weapon icon
    RECT weaponRect = { 0, 20, 32, 52 };
    DirectX::XMFLOAT2 weaponPos(x, y);
    spriteBatch->Draw(_uiTexture, weaponPos, &weaponRect, DirectX::Colors::LightGreen);

    // Vẽ P-số với kích thước nhỏ
    std::string pText = "P-" + FormatNumber(_maxWeapon, 2);
    DrawText(spriteBatch, pText, x + 35, y + 5, DirectX::Colors::Wheat, 1.5f); // Rất nhỏ
}

void Info::release()
{
}

void Info::DrawHealthBar(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch,
    float x, float y, int currentHP, int maxHP, bool isPlayer)
{
    // Kích thước mỗi ô health
    const int HEALTH_WIDTH = 11;
    const int HEALTH_HEIGHT = 18;

    for (int i = 0; i < maxHP; i++)
    {
        RECT sourceRect;

        if (i < currentHP)
        {
            // Vẽ máu đỏ hoặc hồng
            if (isPlayer)
            {
                // red_blood: 0, 0, 11, 18
                sourceRect = { 0, 0, 11, 18 };
            }
            else
            {
                // pink_blood: 22, 0, 33, 18  
                sourceRect = { 22, 0, 33, 18 };
            }
        }
        else
        {
            // empty: 11, 0, 22, 18
            sourceRect = { 11, 0, 22, 18 };
        }

        DirectX::XMFLOAT2 position(x + (i * HEALTH_WIDTH), y);
        spriteBatch->Draw(_uiTexture, position, &sourceRect, DirectX::Colors::White);
    }
}

void Info::DrawText(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch,
    const std::string& text, float x, float y, DirectX::XMVECTOR color = DirectX::Colors::YellowGreen,
    float scale = 1.7f) // Thêm tham số scale, mặc định 0.7 (70% kích thước gốc)
{
    if (!_fontTexture) return;

    const int CHAR_WIDTH = 8;
    const int CHAR_HEIGHT = 8;
    const int CHARS_PER_ROW = 6;

    // Kích thước mới sau khi scale
    float scaledWidth = CHAR_WIDTH * scale;
    float scaledHeight = CHAR_HEIGHT * scale;

    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];
        if (c == ' ') continue;

        int charIndex = -1;

        if (c >= '0' && c <= '9')
        {
            charIndex = c - '0'+26;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            charIndex = (c - 'A');
        }
        else if (c >= 'a' && c <= 'z')
        {
            charIndex = (c - 'a');
        }
        else if (c == '-')
        {
            charIndex = 36;
        }

        if (charIndex >= 0)
        {
            int srcX = (charIndex % CHARS_PER_ROW) * CHAR_WIDTH;
            int srcY = (charIndex / CHARS_PER_ROW) * CHAR_HEIGHT;

            RECT sourceRect = { srcX, srcY, srcX + CHAR_WIDTH, srcY + CHAR_HEIGHT };

            // Sử dụng destination rectangle để scale
            RECT destRect = {
                static_cast<long>(x + (i * (scaledWidth - 2))), // Giảm spacing
                static_cast<long>(y),
                static_cast<long>(x + (i * (scaledWidth - 2)) + scaledWidth),
                static_cast<long>(y + scaledHeight)
            };

            spriteBatch->Draw(_fontTexture, destRect, &sourceRect, color);
        }
    }
}

void Info::DrawLifeIcon(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y)
{
    // Vẽ life icon
    RECT lifeRect = { 34, 0, 50, 16 }; // Vị trí life icon trong texture
    DirectX::XMFLOAT2 lifePos(x, y);
    spriteBatch->Draw(_uiTexture, lifePos, &lifeRect, DirectX::Colors::White);
}

void Info::DrawHearts(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y)
{
    RECT heartRect = { 64, 0, 80, 16 }; // Vị trí heart icon trong texture

    // Hiển thị số hearts hiện tại
    int maxHeartsToShow = 10; // Giới hạn hiển thị tối đa 10 tim
    int heartsToShow = 0;
    if (_heartNumber <= maxHeartsToShow)
    {
        heartsToShow = _heartNumber;
    }
    else
    {
        heartsToShow = maxHeartsToShow; // Hiển thị tối đa 10 tim
    }

    for (int i = 0; i < heartsToShow; i++)
    {
        DirectX::XMFLOAT2 heartPos(x + (i * 18), y + 5); // Cách nhau 18 pixels
        spriteBatch->Draw(_uiTexture, heartPos, &heartRect, DirectX::Colors::White);
    }

    // Nếu có nhiều hơn 10 tim, hiển thị số
    if (_heartNumber > maxHeartsToShow)
    {
        std::string extraHearts = "+" + std::to_string(_heartNumber - maxHeartsToShow);
        DrawText(spriteBatch, extraHearts, x + (maxHeartsToShow * 18) + 10, y + 8);
    }
}

std::string Info::FormatNumber(int number, int width)
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(width) << number;
    return oss.str();
}

// FIX: Sửa cách cập nhật HP
void Info::SetPlayerHitPoint(int number)
{
    _playerHitPointNumber = number;
    if (_playerHitPointNumber > 16)
        _playerHitPointNumber = 16;
    if (_playerHitPointNumber < 0)
        _playerHitPointNumber = 0;
}

int Info::GetPlayerHitPoint()
{
    return _playerHitPointNumber;
}

void Info::SetEnemyHitPoint(int number)
{
    _enemyHitPointNumber = number;
}

int Info::GetEnemyHitPoint()
{
    return _enemyHitPointNumber;
}

void Info::SetHeart(int number)
{
    _heartNumber = number;
    if (_heartNumber >= 100)
        _heartNumber = 99;

    // IMPORTANT: Cập nhật player HP khi heart thay đổi
    _playerHitPointNumber = _heartNumber;
    if (_playerHitPointNumber > 16)
        _playerHitPointNumber = 16;
}

int Info::GetHeart()
{
    return _heartNumber;
}

void Info::AddHeart(int number)
{
    _heartNumber += number;
    if (_heartNumber >= 100)
        _heartNumber = 99;

    // IMPORTANT: Cập nhật player HP khi heart thay đổi
    _playerHitPointNumber = _heartNumber;
    if (_playerHitPointNumber > 16)
        _playerHitPointNumber = 16;
}

void Info::SetLife(int number)
{
    _lifeNumber = number;
}

int Info::GetLife()
{
    return _lifeNumber;
}

void Info::SetStage(int number)
{
    _stageNumber = number;
}

int Info::GetStage()
{
    return _stageNumber;
}

void Info::AddScore(int number)
{
    _scoreNumber += number;
    if (_scoreNumber < 0)
        _scoreNumber = 0; // Đảm bảo score không âm
}

void Info::SetScore(int number)
{
    _scoreNumber = number;
}

int Info::GetScore()
{
    return _scoreNumber;
}

void Info::SetTime(int number)
{
    if (!_pauseTime)
    {
        _timeNumber = number + 1;
        _beginTime = 0; // Thay GameTime::getInstance()->getTotalGameTime();
        time = number;
    }
    else
        time = number;
}

void Info::ActiveTime()
{
    _pauseTime = false;
}

void Info::PauseTime()
{
    _pauseTime = true;
}

int Info::GetTime()
{
    return time;
}

void Info::TakeDamage(int damage)
{
    SetHeart(_heartNumber - damage);
}

void Info::SetMaxWeapon(int num)
{
    _maxWeapon = num;
}

int Info::GetMaxWeapon()
{
    return _maxWeapon;
}

// Hàm để cập nhật thông tin enemy/boss
void Info::SetMaxEnemyHP(int maxHP)
{
    _maxEnemyHP = maxHP;
}

void Info::ShowEnemyBar(bool show)
{
    _showEnemyBar = show;
}

// FIX: Sửa cách cập nhật từ player
void Info::UpdateFromPlayer(Player* player)
{
    if (!player || !player->GetInfo()) return;

    // Cập nhật trực tiếp từ player info thay vì copy
    Info* playerInfo = player->GetInfo();

    // Cập nhật heart và HP dựa trên player's heart
    int playerHearts = playerInfo->GetHeart();
    SetHeart(playerHearts); // Điều này sẽ tự động cập nhật _playerHitPointNumber

    // Cập nhật các thông tin khác
    SetLife(playerInfo->GetLife());
    SetScore(playerInfo->GetScore());
    SetStage(playerInfo->GetStage());
    SetTime(playerInfo->GetTime());
    SetMaxWeapon(playerInfo->GetMaxWeapon());
}