#include "TileMap.h"
#include "WICTextureLoader.h" // Th? vi?n h? tr? load ?nh
#include <d3d11.h>
#include <DirectXMath.h> 

using namespace DirectX;  // Gọi trực tiếp các hàm XM


TileMap::TileMap() : tileWidth(0), tileHeight(0), mapWidth(0), mapHeight(0), screenWidth(0), screenHeight(0),camWidth(0),camHeight(0),cameraX(0) {}


TileMap::TileMap(Render* render, int tileW, int tileH, int screenW, int screenH, int camW,int camH)
    : tileWidth(tileW), tileHeight(tileH), screenWidth(screenW), screenHeight(screenH), cameraX(0) {
    spriteBatch = std::make_unique<DirectX::SpriteBatch>(render->GetDeviceContext());

}

TileMap::~TileMap() {
    if (texture) 
        texture->Release();
}


bool TileMap::LoadMapData(const std::vector<std::vector<int>>& data) {
    if (data.empty()) return false;
    mapData = data;
    mapWidth = data[0].size();
    mapHeight = data.size();
    return true;
}

bool TileMap::LoadTexture(ID3D11Device* device, const wchar_t* filename) {
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filename, nullptr, &texture);
    return SUCCEEDED(hr);
}


// C?p nh?t camera theo v? trí nhân v?t
void TileMap::UpdateCamera(int playerX) {

    cameraX = playerX - camWidth / 2; // Căn giữa nhân vật

    // Giới hạn camera trong bản đồ
    if (cameraX < 0)
        cameraX = 0;
    if (cameraX > mapWidth * tileWidth - camWidth)
        cameraX = mapWidth * tileWidth - camWidth;
}



void TileMap::Draw(Render* render) {
    auto context = render->GetDeviceContext();
    spriteBatch->Begin();
 

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = mapData[y][x];
            if (tileID == -1) continue; // Không vẽ ô trống

            RECT srcRect = { tileID * tileWidth, 0, (tileID + 1) * tileWidth*2, tileHeight };
            XMFLOAT2 pos(x * tileWidth*2 - cameraX, y *2* tileHeight);

            spriteBatch->Draw(texture, pos, &srcRect, Colors::White, 0.0f, XMFLOAT2(0, 0), 2);
           
        }

    }

    spriteBatch->End();
}


