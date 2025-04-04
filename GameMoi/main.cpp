#include <windows.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include "Render.h"
#include "Models/Characters/Player.h"
#include "Models/Characters/Enemy.h"
#include "WICTextureLoader.h"
#include "Tilemap/TileMap.h"
#include "Camera/Camera.h"
#include <algorithm>
using namespace std;
void GameLoop();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const int WIDTH = 800;
const int HEIGHT = 600;

HWND hwnd;
Render renderer;
std::unique_ptr<Player> player;
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

    tileMap = std::make_unique<TileMap>(&renderer, 32, 32);

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

     player = std::make_unique<Player>(200, 128, playerAnimations,device);

    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    string MapData = "Image/tileset.txt";

    if (!tileMap->LoadMapData(MapData)) return false;
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

        float camX = player->GetX()-WIDTH/3;

        // Đảm bảo camera không ra khỏi giới hạn bản đồ
        float leftEdge = 0;
        float rightEdge = tileMap->GetWidth()*32 - WIDTH/3;

        camX = max(leftEdge, min(camX, rightEdge));
        camera->SetPosition(camX, 0);


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
