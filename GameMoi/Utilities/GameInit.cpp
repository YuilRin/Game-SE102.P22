#include "GameInit.h"
#include "DirectXHelpers.h"
#include "WICTextureLoader.h"
#include "../Models/Characters/Enemy/Zombie.h"

HWND hwnd;
Render renderer;
std::unique_ptr<World> world; // Thay thế player và enemyManager bằng World
std::unique_ptr<TileMap> tileMap;
CCamera* camera;

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

    // Enemy positions
    level1.enemyPositions = {
        {200.0f, 300.0f}
    };

    // Item positions
    level1.itemPositions = {
        {300.0f, 250.0f, ItemType::SMALL_HEART}
    };

	level1.breakableItemPositions = {
		{700.0f, 200.0f, BreakableItemType::STAIR},

        {400.0f, 350.0f, BreakableItemType::BIG_CANDLE1},
        {300.0f, 350.0f, BreakableItemType::BIG_CANDLE1}
    };

    if (!SceneBuilder::LoadSceneWithData(world.get(), tileMap.get(), level1, device, context)) {
        return false;
    }

    return true;
}
