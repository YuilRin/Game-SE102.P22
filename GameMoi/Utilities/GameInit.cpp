
#include "GameInit.h"
#include "DirectXHelpers.h"
#include "WICTextureLoader.h"
#include "../Models/Characters/Enemy/Zombie.h"
#include "../Models/Characters/Info.h" // Thêm include

HWND hwnd;
Render renderer;
std::unique_ptr<World> world;
std::unique_ptr<TileMap> tileMap;
std::unique_ptr<Info> gameUI; // Thêm UI
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

    // Khởi tạo World
    world = std::make_unique<World>();
    tileMap = std::make_unique<TileMap>(&renderer, 16, 16);

    // ===== KHỞI TẠO UI =====
    gameUI = std::make_unique<Info>();
    gameUI->init();
    gameUI->LoadTextures(renderer.GetDevice()); // Load UI textures

    // Setup initial UI values
    gameUI->SetScore(0);
    gameUI->SetTime(300); // 5 minutes
    gameUI->SetStage(1);
    gameUI->SetLife(3);
    gameUI->SetHeart(16); // Player starting hearts
    // =====================

    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    //camera
    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    LevelData level1;
    level1.mapFile = "Image/frame0.txt";
    level1.tileTexture = L"Image/frame0tileset.png";
    level1.playerTexture = L"Image/simon.png";
    level1.itemTexture = L"Image/items.png";
    level1.enemyTexture = L"Image/zombie.png";
    level1.breakableItemTexture = L"Image/objects.png";
    level1.startPos = "Image/StartPos/stage21.txt";

    //// Enemy positions
    //level1.enemyPositions = {
    //    {200.0f, 100.0f}
    //};

    //// Item positions
    //level1.itemPositions = {
    //    {300.0f, 150.0f, ItemType::SMALL_HEART}
    //};

    //level1.breakableItemPositions = {
    //   // {700.0f, 100.0f, BreakableItemType::STAIR},
    //    {400.0f, 150.0f, BreakableItemType::BIG_CANDLE1},
    //    {300.0f, 150.0f, BreakableItemType::BIG_CANDLE1}
    //};

    level1.objectPositions = {
         {370.0f, 127.1f, ObjectType::MOVING_STAIR},
         {300.0f, 150.0f, ObjectType::TRIDENT}
    };

    if (!SceneBuilder::LoadSceneWithData(world.get(), tileMap.get(), level1, device, context)) {
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
