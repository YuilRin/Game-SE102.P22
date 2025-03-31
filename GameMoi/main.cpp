#include <windows.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include "Render.h"
#include "Player.h"
#include "Enemy.h"
#include "WICTextureLoader.h"
#include "TileMap.h"
#include "Camera.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const int WIDTH = 800;
const int HEIGHT = 600;

HWND hwnd;
Render renderer;
std::unique_ptr<Player> player;
//std::vector<Enemy> enemies;
std::unique_ptr<TileMap> tileMap;
CCamera* camera;

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

    tileMap = std::make_unique<TileMap>(&renderer, 32, 32, WIDTH, HEIGHT);

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


    player = std::make_unique<Player>(100, 160, playerAnimations,device);

    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    std::vector<std::vector<int>> mapData = {
        {0, 1, 1, 2, 1, 1, 3, 4, 4, 5, 6, 5, 7, 5, 7, 5, 7, 5, 7, 5, 7, 5, 8, 0},
        {0, 1, 9, 10, 9, 1, 3, 8, 8, 11, 3, 11, 3, 11, 3, 11, 3, 11, 3, 11, 3, 11, 12, 0},
        {13, 9, 14, 15, 14, 9, 3, 12, 12, 16, 3, 16, 3, 16, 3, 16, 3, 16, 3, 16, 3, 16, 12, 0},
        {17, 18, 18, 19, 18, 18, 3, 20, 20, 21, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21, 12, 0},
        {23, 23, 23, 23, 23, 23, 23, 24, 24, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 25, 26, 27, 28, 0},
        {29, 29, 29, 29, 29, 29, 29, 29, 29, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 32, 33, 34}
};

    if (!tileMap->LoadMapData(mapData)) return false;
    if (!tileMap->LoadTexture(renderer.GetDevice(), L"Image/tileset.png")) return false;


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
        camera->SetPosition(player->GetX(), player->GetY());

        // Vẽ game
        renderer.BeginRender();
        tileMap->Draw(&renderer,camera);

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
