#pragma once
#include <string>
#include <vector>
#include "../Models/Items/Item.h"
#include "../Models/Items/BreakableItem.h"

struct LevelData {
    std::string mapFile;                         // File .txt tile map
    std::wstring tileTexture;                    // Tileset
    std::wstring playerTexture;                  // Texture của Player
    std::wstring itemTexture;                    // Texture item
    std::wstring enemyTexture;                   // Texture enemy
	std::wstring breakableItemTexture;           // Texture item vỡ
    std::string startPos;                         // File .txt chứa vị trí khởi đầu

    std::vector<std::pair<float, float>> enemyPositions;
    std::vector<std::tuple<float, float, ItemType>> itemPositions;
	std::vector<std::tuple<float, float, BreakableItemType>> breakableItemPositions;
	std::vector<std::tuple<float, float, ObjectType>> objectPositions; // Vị trí của các Object
	//std::vector<std::pair<float, float>> stairPositions; // Vị trí của các cầu thang
};
