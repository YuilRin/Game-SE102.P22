#include <windows.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include "Render.h"
#include "Player.h"
#include "Enemy.h"
#include "WICTextureLoader.h"
#include "TileMap.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const int WIDTH = 800;
const int HEIGHT = 600;

HWND hwnd;
Render renderer;
std::unique_ptr<Player> player;
//std::vector<Enemy> enemies;
std::unique_ptr<TileMap> tileMap;


bool InitGame(HINSTANCE hInstance, int nCmdShow) {
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectXGame";
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, L"DirectXGame", L"DirectX 11 Game",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    if (!hwnd) 
        return false;

    ShowWindow(hwnd, nCmdShow);


    if (!renderer.Init(hwnd, WIDTH, HEIGHT)) return false;

    tileMap = std::make_unique<TileMap>(&renderer, 128, 192, WIDTH, HEIGHT, 256, 192);

    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    // === Load textures cho Player ===
    ID3D11ShaderResourceView* spriteSheet = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, L"Image/simon.jpg", nullptr, &spriteSheet);

    std::map<PlayerState, Animation> playerAnimations = {
    {PlayerState::Idle, Animation(spriteSheet, {{585, 0, 615, 64}}, 0.15f)},  // Loop
    {PlayerState::Walking, Animation(spriteSheet, {{585, 0, 615, 64}, {618, 0, 640, 64}, {643, 0, 673, 64}}, 0.15f)},  // Loop
    {PlayerState::Jumping, Animation(spriteSheet, {{429, 0, 459, 48}}, 0.15f)},  // Không loop
    {PlayerState::SitDown, Animation(spriteSheet, {{429, 0, 459, 48}}, 0.15f)},
        {PlayerState::Stand_Hit, Animation(spriteSheet,{{460,	0,	507,	64},{510,	0,	541,	64},{542,	0,	584,	64}},0.3f)}
    };


    player = std::make_unique<Player>(10, 160, playerAnimations,device);


    // === Load textures cho Enemy ===
    //std::vector<ID3D11ShaderResourceView*> enemyLeftFrames;
    //std::vector<ID3D11ShaderResourceView*> enemyRightFrames;

    //ID3D11ShaderResourceView* enemy1 = nullptr;
    //ID3D11ShaderResourceView* enemy2 = nullptr;
    //ID3D11ShaderResourceView* enemy3 = nullptr;
    //ID3D11ShaderResourceView* enemy4 = nullptr;
    //ID3D11ShaderResourceView* enemy13 = nullptr;
    //ID3D11ShaderResourceView* enemy23 = nullptr;
    //ID3D11ShaderResourceView* enemy33 = nullptr;
    //ID3D11ShaderResourceView* enemy43= nullptr;

    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-1.png", nullptr, &enemy1);
    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-2.png", nullptr, &enemy2);
    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-3.png", nullptr, &enemy3);
    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-4.png", nullptr, &enemy4);

    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-1-R.png", nullptr, &enemy13);
    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-2-R.png", nullptr, &enemy23);
    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-3-R.png", nullptr, &enemy33);
    //DirectX::CreateWICTextureFromFile(device, context, L"Image/enemy-4-R.png", nullptr, &enemy43);

    //enemyLeftFrames.push_back(enemy1);
    //enemyLeftFrames.push_back(enemy2);
    //enemyLeftFrames.push_back(enemy3);
    //enemyLeftFrames.push_back(enemy4);
    //enemyRightFrames.push_back(enemy13);
    //enemyRightFrames.push_back(enemy23);
    //enemyRightFrames.push_back(enemy33);
    //enemyRightFrames.push_back(enemy43);

    //// Tạo danh sách Enemy
    //enemies.emplace_back(100, 200, enemyLeftFrames, enemyRightFrames, 0.2f);
    //enemies.emplace_back(300, 400, enemyLeftFrames, enemyRightFrames, 0.2f);

    std::vector<std::vector<int>> mapData = {
        {0,1,2,3,4,5}
};

    if (!tileMap->LoadMapData(mapData)) return false;
    if (!tileMap->LoadTexture(renderer.GetDevice(), L"Image/frame1.png")) return false;


    return true;
}

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
        tileMap->UpdateCamera(player->GetX());

        // Vẽ game
        renderer.BeginRender();
        tileMap->Draw(&renderer);

        player->Render(renderer.GetSpriteBatch());
        renderer.EndRender();
    }
}

// Xử lý đầu vào bàn phím
void HandleInput(WPARAM key) {
    player->HandleInput(key);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (!InitGame(hInstance, nCmdShow)) 
        return 0;
    GameLoop();
    return 0;
}
