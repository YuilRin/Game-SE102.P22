#include "TileMap.h"
#include "WICTextureLoader.h" // Th? vi?n h? tr? load ?nh
#include <d3d11.h>
#include <DirectXMath.h> 
using namespace DirectX;  // Gọi trực tiếp các hàm XM


TileMap::TileMap(Render* render, int tileW, int tileH, int screenW, int screenH)
    : tileWidth(tileW), tileHeight(tileH), screenWidth(screenW), screenHeight(screenH), cameraX(0) {
    spriteBatch = std::make_unique<DirectX::SpriteBatch>(render->GetDeviceContext());
}

TileMap::~TileMap() {
    if (texture) texture->Release();
}

// Load ma tr?n map (ch?a ch? s? các tile)
bool TileMap::LoadMapData(const std::vector<std::vector<int>>& data) {
    if (data.empty()) return false;
    mapData = data;
    mapWidth = data[0].size();
    mapHeight = data.size();
    return true;
}

// Load texture map
bool TileMap::LoadTexture(ID3D11Device* device, const wchar_t* filename) {
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filename, nullptr, &texture);
    return SUCCEEDED(hr);
}

// C?p nh?t camera theo v? trí nhân v?t
void TileMap::UpdateCamera(int playerX) {
    cameraX = playerX - screenWidth / 2; // Camera di chuy?n theo nhân v?t
    if (cameraX < 0) cameraX = 0;
    if (cameraX > mapWidth * tileWidth - screenWidth) cameraX = mapWidth * tileWidth - screenWidth;
}

void TileMap::Draw(Render* render) {
    auto context = render->GetDeviceContext();
    spriteBatch->Begin();

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = mapData[y][x];
            if (tileID == -1) continue; // Không vẽ ô trống

            RECT srcRect = { tileID * tileWidth, 0, (tileID + 1) * tileWidth, tileHeight };
            XMFLOAT2 pos(x * tileWidth - cameraX, y * tileHeight);

            spriteBatch->Draw(texture, pos, &srcRect);
        }
    }

    spriteBatch->End();
}