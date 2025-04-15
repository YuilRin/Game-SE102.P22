#include "GameInit.h"
#include "DirectXHelpers.h" 
#include "WICTextureLoader.h"


HWND hwnd;
Render renderer;
std::unique_ptr<Player> player;
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

    tileMap = std::make_unique<TileMap>(&renderer, 16, 16);
    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    // Tải sprite sheet của nhân vật
    ID3D11ShaderResourceView* spriteSheet = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, L"Image/simon.jpg", nullptr, &spriteSheet);

    // Tạo danh sách animation của player
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

    player = std::make_unique<Player>(30, 428, playerAnimations, device);

    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    string MapData = "Image/frame0.txt";
    if (!tileMap->LoadMapData(MapData)) return false;
    if (!tileMap->LoadTexture(renderer.GetDevice(), L"Image/frame0tileset.png")) return false;


    std::vector<std::vector<int>> rawMap = tileMap->GetMapData(); // thêm hàm này nếu chưa có

   
    for (auto& row : rawMap) {
        for (auto& tile : row) {
            if (tile == 0 || tile == 7 || tile == 8 || tile == 17 || tile == 18)
                tile = 0;// tile gạch
            else
                if (tile == 4 || tile == 14||tile==27)
                    tile = 1;//cầu thang
                else if (tile == 27)// đỉnh cầu thang
                    tile = 2;
                else
                    tile = -1;
        }
    }

    std::vector<Collider*> groundColliders = CreateOptimizedCollidersFromTileMap(rawMap, tileMap->GetTileSize(),0);

    // Gán collider cho Player
    player->SetGroundColliders(groundColliders);

    std::vector<Collider*> stairColliders = CreateStairCollidersFromTileMap(rawMap, tileMap->GetTileSize());
    player->SetStairColliders(stairColliders);
  

    return true;
}
