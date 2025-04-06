#include "GameInit.h"
#include "DirectXHelpers.h" 
#include "WICTextureLoader.h"


HWND hwnd;
Render renderer;
std::unique_ptr<Player> player;
std::unique_ptr<TileMap> tileMap;
CCamera* camera;


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

    tileMap = std::make_unique<TileMap>(&renderer, 32, 32);
    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    // Tải sprite sheet của nhân vật
    ID3D11ShaderResourceView* spriteSheet = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, L"Image/simon.jpg", nullptr, &spriteSheet);

    // Tạo danh sách animation của player
    std::map<PlayerState, Animation> playerAnimations = {
        {PlayerState::Idle, Animation(spriteSheet, {{585, 0, 615, 64}}, 0.15f)},
        {PlayerState::Walking, Animation(spriteSheet, {{585, 0, 615, 64}, {618, 0, 640, 64}, {643, 0, 673, 64}}, 0.15f)},
        {PlayerState::Jumping, Animation(spriteSheet, {{429, 0, 459, 48}}, 0.15f)},
        {PlayerState::SitDown, Animation(spriteSheet, {{429, 0, 459, 48}}, 0.15f)},
        {PlayerState::Stand_Hit, Animation(spriteSheet,{{460, 0, 507, 64},{510, 0, 541, 64},{542, 0, 584, 64}},0.3f)}
    };

    player = std::make_unique<Player>(100, 128, playerAnimations, device);

    camera = CCamera::GetInstance();
    camera->Init();
    camera->SetSize(WIDTH, HEIGHT);

    string MapData = "Image/tileset.txt";
    if (!tileMap->LoadMapData(MapData)) return false;
    if (!tileMap->LoadTexture(renderer.GetDevice(), L"Image/tileset.png")) return false;


    std::vector<std::vector<int>> rawMap = tileMap->GetMapData(); // thêm hàm này nếu chưa có

    // Chuyển tile 23 thành 0 để dễ xử lý collider
    for (auto& row : rawMap) {
        for (auto& tile : row) {
            if (tile == 23)
                tile = 0;
            else
                tile = -1; // hoặc giá trị nào không phải tile gạch
        }
    }

    std::vector<Collider> groundColliders = Collider::CreateCollisionObjects(rawMap, tileMap->GetTileSize());

    // Gán collider cho Player
    player->SetGroundColliders(groundColliders);
   

    return true;
}
