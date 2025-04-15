// Collider.h
#ifndef STAIRCOLLIDER_H
#define STAIRCOLLIDER_H

#include <vector>
#include <algorithm>
#include "./Collider.h"
#pragma once
#include "Collider.h"

enum class StairDirection {
    LeftUp,
    RightUp
};

class StairCollider : public Collider {
public:
    StairDirection direction;
    bool isTop; 
    bool temporarilyDisabled = false; // mới thêm

    StairCollider(float x, float y, float w, float h, StairDirection dir, bool top)
        : Collider(x, y, w, h, 0.0f, 0.0f, false), direction(dir), isTop(top) {
    }

    StairDirection GetDirection() const { return direction; }
    bool IsTop() const { return isTop; }
    void SetTemporarilyDisabled(bool value) { temporarilyDisabled = value; }
    bool IsTemporarilyDisabled() const { return temporarilyDisabled; }
};

#endif // STAIRCOLLIDER_H