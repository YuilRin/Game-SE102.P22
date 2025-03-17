#include <windows.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include "Render.h"
#include "Player.h"
#include "Enemy.h"
#include "WICTextureLoader.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Kích thước cửa sổ
const int WIDTH = 800;
const int HEIGHT = 600;

// Biến toàn cục
HWND hwnd;
Render renderer;
std::unique_ptr<Player> player;
std::vector<Enemy> enemies;

// Hàm khởi tạo DirectX và các đối tượng game
bool InitGame(HINSTANCE hInstance, int nCmdShow) {
    // Đăng ký lớp cửa sổ
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectXGame";
    RegisterClass(&wc);

    // Tạo cửa sổ
    hwnd = CreateWindowEx(0, L"DirectXGame", L"DirectX 11 Game",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    if (!hwnd) return false;
    ShowWindow(hwnd, nCmdShow);

    // Khởi tạo DirectX
    if (!renderer.Init(hwnd, WIDTH, HEIGHT)) return false;

    // Lấy thiết bị Direct3D từ Render
    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    // === Load textures cho Player ===
    std::vector<ID3D11ShaderResourceView*> playerLeftFrames;
    std::vector<ID3D11ShaderResourceView*> playerRightFrames;

    ID3D11ShaderResourceView* player1 = nullptr;
    ID3D11ShaderResourceView* player2 = nullptr;
    ID3D11ShaderResourceView* player3 = nullptr;
    ID3D11ShaderResourceView* player4 = nullptr;

    DirectX::CreateWICTextureFromFile(device, context, L"player.png", nullptr, &player1);
    DirectX::CreateWICTextureFromFile(device, context, L"player.png", nullptr, &player2);
    DirectX::CreateWICTextureFromFile(device, context, L"player.png", nullptr, &player3);
    DirectX::CreateWICTextureFromFile(device, context, L"player.png", nullptr, &player4);

    playerLeftFrames.push_back(player1);
    playerLeftFrames.push_back(player2);
    playerRightFrames.push_back(player3);
    playerRightFrames.push_back(player4);

    // Tạo Player với ảnh động
    player = std::make_unique<Player>(100.0f, 300.0f, playerLeftFrames, playerRightFrames, 0.15f);

    // === Load textures cho Enemy ===
    std::vector<ID3D11ShaderResourceView*> enemyLeftFrames;
    std::vector<ID3D11ShaderResourceView*> enemyRightFrames;

    ID3D11ShaderResourceView* enemy1 = nullptr;
    ID3D11ShaderResourceView* enemy2 = nullptr;
    ID3D11ShaderResourceView* enemy3 = nullptr;
    ID3D11ShaderResourceView* enemy4 = nullptr;

    DirectX::CreateWICTextureFromFile(device, context, L"enemy1.png", nullptr, &enemy1);
    DirectX::CreateWICTextureFromFile(device, context, L"enemy2.png", nullptr, &enemy2);
    DirectX::CreateWICTextureFromFile(device, context, L"enemy1.png", nullptr, &enemy3);
    DirectX::CreateWICTextureFromFile(device, context, L"enemy2.png", nullptr, &enemy4);

    enemyLeftFrames.push_back(enemy1);
    enemyLeftFrames.push_back(enemy2);
    enemyRightFrames.push_back(enemy3);
    enemyRightFrames.push_back(enemy4);

    // Tạo danh sách Enemy
    enemies.emplace_back(100, 200, enemyLeftFrames, enemyRightFrames, 0.2f);
    enemies.emplace_back(300, 400, enemyLeftFrames, enemyRightFrames, 0.2f);

    return true;
}

// Vòng lặp game chính
void GameLoop() {
    MSG msg = { 0 };
    DWORD prevTime = GetTickCount();

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DWORD currentTime = GetTickCount();
        float deltaTime = (currentTime - prevTime) / 1000.0f;
        prevTime = currentTime;

        // Cập nhật game
        player->Update(deltaTime);
        for (auto& enemy : enemies) {
            enemy.Update(deltaTime);
        }

        // Vẽ game
        renderer.BeginRender();
        player->Render(renderer.GetSpriteBatch());
        for (auto& enemy : enemies) {
            enemy.Render(renderer.GetSpriteBatch());
        }
        renderer.EndRender();
    }
}

// Xử lý đầu vào bàn phím
void HandleInput(WPARAM key) {
    player->HandleInput(key);
}

// Xử lý sự kiện cửa sổ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        HandleInput(wParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Điểm vào chính của chương trình
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (!InitGame(hInstance, nCmdShow)) return 0;
    GameLoop();
    return 0;
}
