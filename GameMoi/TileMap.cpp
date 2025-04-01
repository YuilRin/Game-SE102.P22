#include "TileMap.h"
#include "WICTextureLoader.h" // Th? vi?n h? tr? load ?nh
#include <d3d11.h>
#include <DirectXMath.h> 
#include <fstream>
#include <sstream>
using namespace DirectX;  // Gọi trực tiếp các hàm XM


TileMap::TileMap() : tileWidth(0), tileHeight(0), mapWidth(0), mapHeight(0) {}


TileMap::TileMap(Render* render, int tileW, int tileH, int screenW, int screenH)
    : tileWidth(tileW), tileHeight(tileH) {
    spriteBatch = std::make_unique<DirectX::SpriteBatch>(render->GetDeviceContext());

}

TileMap::~TileMap() {
    if (texture) 
        texture->Release();
}


bool TileMap::LoadMapData(string &filePath) {
    ifstream file(filePath);
    std::vector<std::vector<int>> data;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> row;
        int value;
        while (ss >> value) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    file.close();
    if (data.empty() || data[0].empty()) return false;

    size_t width = data[0].size();
    for (const auto& row : data) {
        if (row.size() != width) {
            return false;
        }
    }

    mapData = data;
    mapWidth = width;
    mapHeight = data.size();
    return true;
}


bool TileMap::LoadTexture(ID3D11Device* device, const wchar_t* filename) {
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filename, nullptr, &texture);
    return SUCCEEDED(hr);
}



void TileMap::Draw(Render* render,CCamera* camera) {
    auto context = render->GetDeviceContext();
    spriteBatch->Begin();


    float camX= camera->GetLeft();
    

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = mapData[y][x];
            if (tileID == -1) continue; // Không vẽ ô trống
            float scale = 3.125f;
            RECT srcRect = { tileID  *  tileWidth, 0, (tileID + 1)  * tileWidth  , tileHeight };
            XMFLOAT2 pos(x * scale* tileWidth - scale * camX , y   * scale * tileHeight);
            spriteBatch->Draw(texture, pos, &srcRect, Colors::White, 0.0f, XMFLOAT2(0, 0),scale);
           
        }

    }

    spriteBatch->End();
}

int TileMap::GetWidth()
{
    return mapWidth*32;
}
int TileMap::GetHeight()
{
    return mapHeight*32;
}

