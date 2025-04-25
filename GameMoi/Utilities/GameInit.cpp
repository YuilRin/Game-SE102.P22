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

    // ================== TẢI TEXTURE VÀ TẠO PLAYER ==================
    ID3D11ShaderResourceView* spriteSheet = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, L"Image/simon.jpg", nullptr, &spriteSheet);

    std::map<PlayerState, Animation> playerAnimations = {
        {PlayerState::Idle     , Animation(spriteSheet, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::Jumping  , Animation(spriteSheet, {{429, 0 , 455, 48}}, 0.3f)},
        {PlayerState::SitDown  , Animation(spriteSheet, {{429, 0 , 455, 48}}, 0.3f)},
        {PlayerState::Walking  , Animation(spriteSheet, {{585, 0 , 615, 64} , {618, 0 , 640, 64 }, {643,  0, 673, 64 }}, 0.3f)},
        {PlayerState::Stand_Hit, Animation(spriteSheet, {{460, 0 , 507, 64} , {510, 0 , 541, 64 }, {542,  0, 584, 64 }}, 0.3f)},
        {PlayerState::Climbing , Animation(spriteSheet, {{421, 64, 453, 123}, {392,	64,	421, 123}, {365, 64, 394, 123}}, 0.3f)},
        {PlayerState::Up_Hit   , Animation(spriteSheet, {{96 , 0 , 144,	64} , {144, 0 ,	176, 64 }, {176,  0, 216, 64 }}, 0.3f)},
        {PlayerState::Down_Hit , Animation(spriteSheet, {{216, 0 , 258,	64} , {258, 0 , 289, 64 }, {290,  0, 334, 4  }}, 0.3f)}
    };

    auto player = std::make_unique<Player>(30, 428, playerAnimations, device);
    world->SetPlayer(std::move(player));

    // ================== XỬ LÝ TILEMAP VÀ COLLIDER ==================
    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    std::string MapData = "Image/frame0.txt";
    if (!tileMap->LoadMapData(MapData)) return false;
    if (!tileMap->LoadTexture(device, L"Image/frame0tileset.png")) return false;

    std::vector<std::vector<int>> rawMap = tileMap->GetMapData();

    // Xử lý tilemap
    for (auto& row : rawMap) {
        for (auto& tile : row) {
            if (tile == 0 || tile == 7 || tile == 8 || tile == 17 || tile == 18 || tile == 9)
                tile = 0;// tile gạch
            else if (tile == 4 || tile == 14 || tile == 27)
                tile = 1;//cầu thang
            else if (tile == 27)// đỉnh cầu thang
                tile = 2;
            else
                tile = -1;
        }
    }

    // Tạo collider
    std::vector<Collider*> groundColliders = CreateOptimizedCollidersFromTileMap(rawMap, tileMap->GetTileSize(), 0);
    world->GetPlayer()->SetGroundColliders(groundColliders);

    std::vector<Collider*> stairColliders = CreateStairCollidersFromTileMap(rawMap, tileMap->GetTileSize());
    world->GetPlayer()->SetStairColliders(stairColliders);

    // ================== THÊM ITEM VÀ ENEMY ==================
    ID3D11ShaderResourceView* itemTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, L"Image/items.png", nullptr, &itemTexture);
    world->AddItem(std::make_unique<Item>(300.0f, 150.0f, ItemType::SMALL_HEART, itemTexture));

    ID3D11ShaderResourceView* zombieTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, L"Image/zombie.png", nullptr, &zombieTexture);

    // Thêm zombie vào World
    Zombie* zombie1 = new Zombie(200.0f, 300.0f, zombieTexture);
    zombie1->LoadAnimations(device);
    zombie1->SetPlayerReference(world->GetPlayer());
    world->AddEnemy(std::unique_ptr<Enemy>(zombie1));

    return true;
}
