#pragma once
#include "../Models/World.h"
#include "../Tilemap/Tilemap.h"
#include "../Render.h"
#include "LevelData.h"

class SceneBuilder {
public:
    static bool LoadSceneWithData(
        World* world,
        TileMap* tileMap,
        const LevelData& data,
        ID3D11Device* device,
        ID3D11DeviceContext* context
    );
};
