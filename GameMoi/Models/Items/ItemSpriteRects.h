#pragma once
#include <map>
#include <string>
#include <Windows.h>
#include "Item.h" // enum class ItemType

class ItemSpriteRects {
public:
    static std::map<ItemType, RECT> GetRects();
};
