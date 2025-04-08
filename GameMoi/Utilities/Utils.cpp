#include "Utils.h"

bool isIntersected(RECT a, RECT b)
{
    if (a.right <= b.left || a.left >= b.right ||
        a.top <= b.bottom || a.bottom >= b.top)
        return false;
    return true;
}

bool isIntersectedInGame(RECT a, RECT b)
{
    float left = a.right - b.left;
    float right = b.right - a.left;
    float top = a.top - b.bottom;
    float bottom = b.top - a.bottom;

    return (left > 0 && right > 0 && top > 0 && bottom > 0);
}
