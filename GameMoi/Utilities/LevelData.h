#pragma once
#include <string>
#include <vector>
#include "../Models/Items/Item.h"

struct LevelData {
    std::string mapFile;                         // File .txt tile map
    std::wstring tileTexture;                    // Tileset
    std::wstring playerTexture;                  // Texture của Player
    std::wstring itemTexture;                    // Texture item
    std::wstring enemyTexture;                   // Texture enemy

    std::vector<std::pair<float, float>> enemyPositions;
    std::vector<std::tuple<float, float, ItemType>> itemPositions;
};
