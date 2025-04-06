#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include "../Camera/Camera.h"
#include "../Render.h"
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
    TileMap(Render* render, int tileW, int tileH);
    ~TileMap();
       bool LoadMapData(string &filePath);
    bool LoadTexture(ID3D11Device* device, const wchar_t* filename);
    std::vector<std::vector<int>> LoadTileSet(int tileW,int tileH, const wchar_t* filename);
    void Draw(Render* render,CCamera* camera);
    int GetWidth();
    int GetHeight();

    ////////////////////////////////////////////////
    int GetTileSize() { return 100.0f; };
    std::vector<std::vector<int>> GetMapData() { return mapData; }

	
	ID3D11ShaderResourceView* GetTexture() { return texture; }
	int GetTileWidth() { return tileWidth; }
	int GetTileHeight() { return tileHeight; }
	int GetMapWidth() { return mapWidth; }
	int GetMapHeight() { return mapHeight; }
	void SetMapFile(const string& file) { mapFile = file; }
	void SetTilesetFile(const string& file) { tilesetFile = file; }
};

#endif // TILEMAP_H
