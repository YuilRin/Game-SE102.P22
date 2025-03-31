#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include "Camera.h"
#include "Render.h"
#include <vector>
#include <string>
#include <memory>
#include <string>
using namespace std;

class TileMap {
private:
    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    ID3D11ShaderResourceView* texture = nullptr; // Texture của map
    std::vector<std::vector<int>> mapData; // Ma trận lưu các ô tile

    int tileWidth;
    int tileHeight;
    int mapWidth;
    int mapHeight;
    string mapFile;
    string tilesetFile;

public:
   TileMap();
    TileMap(Render* render, int tileW, int tileH, int screenW, int screenH);
    ~TileMap();
       bool LoadMapData(const std::vector<std::vector<int>>& data);
    bool LoadTexture(ID3D11Device* device, const wchar_t* filename);
    std::vector<std::vector<int>> LoadTileSet(int tileW,int tileH, const wchar_t* filename);
    void Draw(Render* render,CCamera* camera);
};

#endif // TILEMAP_H
