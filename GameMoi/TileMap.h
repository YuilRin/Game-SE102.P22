#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include "Render.h"
#include <vector>
#include <memory>

class TileMap {
private:
    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    ID3D11ShaderResourceView* texture = nullptr; // Texture của map
    std::vector<std::vector<int>> mapData; // Ma trận lưu các ô tile

    int tileWidth;
    int tileHeight;
    int mapWidth;
    int mapHeight;
    int screenWidth;
    int screenHeight;
    int cameraX; // Vị trí của camera theo trục X

public:
   TileMap();
    TileMap(Render* render, int tileW, int tileH, int screenW, int screenH);
    ~TileMap();
       bool LoadMapData(const std::vector<std::vector<int>>& data);
    bool LoadTexture(ID3D11Device* device, const wchar_t* filename);
    void UpdateCamera(int playerX);
    void Draw(Render* render);
};

#endif // TILEMAP_H
