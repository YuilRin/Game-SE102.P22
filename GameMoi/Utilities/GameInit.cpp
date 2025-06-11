#include "GameInit.h"
#include "DirectXHelpers.h"
#include "WICTextureLoader.h"
#include "../Models/Characters/Enemy/Zombie.h"
#include "../Models/Characters/Info.h"

HWND hwnd;
Render renderer;
std::unique_ptr<World> world;
std::unique_ptr<TileMap> tileMap;
std::unique_ptr<Info> gameUI;
std::unique_ptr<LevelManager> levelManager; // Thêm LevelManager
CCamera* camera;

std::function<void(int)> OnEnemyKilled = nullptr;
std::function<void(int)> OnItemCollected = nullptr;

enum TileType {
    TILE_EMPTY = -1,
    TILE_SOLID = 0,
    TILE_STAIR = 1,
    TILE_STAIR_TOP = 2
};

bool InitGame(HINSTANCE hInstance, int nCmdShow) {
    // Khởi tạo cửa sổ
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectXGame";
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, L"DirectXGame", L"DirectX 11 Game",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);
    if (!hwnd) return false;

    ShowWindow(hwnd, nCmdShow);

    // Khởi tạo DirectX
    if (!renderer.Init(hwnd, WIDTH, HEIGHT)) return false;

    // Khởi tạo World và LevelManager
    world = std::make_unique<World>();
    tileMap = std::make_unique<TileMap>(&renderer, 16, 16);
    levelManager = std::make_unique<LevelManager>(); // Khởi tạo LevelManager

    // ===== KHỞI TẠO UI =====
    gameUI = std::make_unique<Info>();
    gameUI->init();
    gameUI->LoadTextures(renderer.GetDevice()); // Load UI textures

    // Setup initial UI values
    gameUI->SetScore(0);
    gameUI->SetTime(300); // 5 minutes
    gameUI->SetStage(levelManager->GetCurrentLevelNumber()); // Sử dụng level number từ manager
    gameUI->SetLife(3);
    gameUI->SetHeart(16); // Player starting hearts
    // =====================

    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    //camera
    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    // Load level đầu tiên từ LevelManager
    const LevelData& currentLevel = levelManager->GetCurrentLevel();

    if (!SceneBuilder::LoadSceneWithData(world.get(), tileMap.get(), currentLevel, device, context)) {
        return false;
    }

    OnEnemyKilled = [](int points) {
        if (gameUI) {
            gameUI->AddScore(points);
        }
        };

    OnItemCollected = [](int heartValue) {
        if (gameUI) {
            gameUI->AddHeart(heartValue);
        }
        };

    return true;
}