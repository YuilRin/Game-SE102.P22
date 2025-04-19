#include "ItemSpriteRects.h"

std::map<ItemType, RECT> ItemSpriteRects::GetRects()
{
    std::map<ItemType, RECT> rects;

    rects[ItemType::SMALL_HEART] = { 0, 0, 16, 16 };
    rects[ItemType::BIG_HEART] = { 16, 0, 40, 20 };
    rects[ItemType::AXE] = { 256, 0, 286, 28 };
    rects[ItemType::POT_ROAST] = { 62, 32, 94, 58 };
    // ...

    return rects;
}
