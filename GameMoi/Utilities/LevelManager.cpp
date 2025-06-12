#include "LevelManager.h"

LevelManager::LevelManager() : currentLevelIndex(0), maxLevels(0) {
    InitializeLevels();
}

void LevelManager::InitializeLevels() {
    levels.clear();

    // Thêm tất cả các level vào vector
    levels.push_back(CreateLevel1());
    levels.push_back(CreateLevel2());
    levels.push_back(CreateLevel3());
    levels.push_back(CreateLevel4());
    levels.push_back(CreateLevel5());

    maxLevels = static_cast<int>(levels.size());
    currentLevelIndex = 0;
}

const LevelData& LevelManager::GetCurrentLevel() const {
    return levels[currentLevelIndex];
}

bool LevelManager::NextLevel() {
    if (HasNextLevel()) {
        currentLevelIndex++;
        return true;
    }
    return false; // Đã hết level
}

LevelData LevelManager::CreateLevel1() {
    LevelData level1;
    level1.mapFile = "Image/frame0.txt";
    level1.tileTexture = L"Image/frame0tileset.png";
    level1.playerTexture = L"Image/simon.png";
    level1.itemTexture = L"Image/items.png";
    level1.enemyTexture = L"Image/zombie.png";
    level1.breakableItemTexture = L"Image/objects.png";
    level1.startPos = "Image/StartPos/stage21.txt";

    level1.enemyPositions = {
       {300.0f, 200.0f}
    };
    level1.objectPositions = {
        {370.0f, 127.1f, ObjectType::MOVING_STAIR},
       // {300.0f, 150.0f, ObjectType::TRIDENT}
    };

    return level1;
}

LevelData LevelManager::CreateLevel2() {
    LevelData level2;
    level2.mapFile = "Image/frame3.txt";
    level2.tileTexture = L"Image/frame3tileset.png";
    level2.playerTexture = L"Image/simon.png";
    level2.itemTexture = L"Image/items.png";
    level2.enemyTexture = L"Image/zombie.png";
    level2.breakableItemTexture = L"Image/objects.png";
    level2.startPos = "Image/StartPos/stage22.txt";

    level2.enemyPositions = {
        {100.0f, 100.0f}
    };

    level2.itemPositions = {
        {200.0f, 250.0f, ItemType::SMALL_HEART}
    };

    level2.breakableItemPositions = {
        {700.0f, 200.0f, BreakableItemType::STAIR},
        {400.0f, 350.0f, BreakableItemType::BIG_CANDLE1},
        {300.0f, 350.0f, BreakableItemType::BIG_CANDLE1}
    };

    return level2;
}

LevelData LevelManager::CreateLevel3() {
    LevelData level3;
    level3.mapFile = "Image/frame1.txt"; // Giả sử bạn có map này
    level3.tileTexture = L"Image/frame1tileset.png";
    level3.playerTexture = L"Image/simon.png";
    level3.itemTexture = L"Image/items.png";
    level3.enemyTexture = L"Image/zombie.png";
    level3.breakableItemTexture = L"Image/objects.png";
    level3.startPos = "Image/StartPos/stage23.txt";

    level3.enemyPositions = {
        {150.0f, 100.0f},
        {250.0f, 100.0f}
    };

    level3.itemPositions = {
        {300.0f, 200.0f, ItemType::SMALL_HEART},
        {400.0f, 200.0f, ItemType::SMALL_HEART}
    };

    level3.breakableItemPositions = {
        {500.0f, 150.0f, BreakableItemType::BIG_CANDLE1},
        {600.0f, 150.0f, BreakableItemType::BIG_CANDLE1}
    };

    level3.objectPositions = {
        {450.0f, 120.0f, ObjectType::TRIDENT}
    };

    return level3;
}

LevelData LevelManager::CreateLevel4() {
    LevelData level4;
    level4.mapFile = "Image/frame2.txt";
    level4.tileTexture = L"Image/frame2tileset.png";
    level4.playerTexture = L"Image/simon.png";
    level4.itemTexture = L"Image/items.png";
    level4.enemyTexture = L"Image/zombie.png";
    level4.breakableItemTexture = L"Image/objects.png";
    level4.startPos = "Image/StartPos/stage24.txt";

    // Level 4 - khó hơn với nhiều enemy
    level4.enemyPositions = {
        {100.0f, 100.0f},
        {200.0f, 100.0f},
        {300.0f, 100.0f}
    };

    level4.itemPositions = {
        {150.0f, 200.0f, ItemType::SMALL_HEART},
        {350.0f, 200.0f, ItemType::SMALL_HEART}
    };

    level4.breakableItemPositions = {
        {400.0f, 100.0f, BreakableItemType::STAIR},
        {500.0f, 150.0f, BreakableItemType::BIG_CANDLE1}
    };

    level4.objectPositions = {
        {250.0f, 130.0f, ObjectType::MOVING_STAIR},
        {400.0f, 160.0f, ObjectType::TRIDENT}
    };

    return level4;
}

LevelData LevelManager::CreateLevel5() {
    LevelData level5;
    level5.mapFile = "Image/frame4.txt"; // Boss level
    level5.tileTexture = L"Image/frame4tileset.png";
    level5.playerTexture = L"Image/simon.png";
    level5.itemTexture = L"Image/items.png";
    level5.enemyTexture = L"Image/boss.png"; // Boss texture
    level5.breakableItemTexture = L"Image/objects.png";
    level5.startPos = "Image/StartPos/stage25.txt";

    // Boss level - ít enemy nhưng mạnh
    level5.enemyPositions = {
        {300.0f, 100.0f} // Boss position
    };

    level5.itemPositions = {
        {100.0f, 200.0f, ItemType::SMALL_HEART},
        {200.0f, 200.0f, ItemType::SMALL_HEART},
        {400.0f, 200.0f, ItemType::SMALL_HEART}
    };

    level5.breakableItemPositions = {
        {150.0f, 150.0f, BreakableItemType::BIG_CANDLE1},
        {350.0f, 150.0f, BreakableItemType::BIG_CANDLE1}
    };

    level5.objectPositions = {
        {250.0f, 180.0f, ObjectType::TRIDENT}
    };

    return level5;
}