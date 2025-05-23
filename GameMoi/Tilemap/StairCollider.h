// Collider.h
#ifndef STAIRCOLLIDER_H
#define STAIRCOLLIDER_H

#include <vector>
#include <algorithm>
#pragma once
#include "Collider.h"

enum class StairDirection {
    LeftUp,
    RightUp
};

class StairCollider : public Collider {
private:
    StairDirection direction;
    bool isTop;
    bool shouldBlockThisFrame;
public:

    StairDirection GetDirection() const { return direction; }
    bool IsTop() const { return isTop; }

    // Phương thức để set blocking cho frame hiện tại
    void SetFrameBlocking(bool shouldBlock);

    // Reset về trạng thái blocking mặc định
    void ResetFrameBlocking();

    bool IsBlocking() const;

    void SetBlocking(bool shouldBlock);

  

    bool temporarilyDisabled = false; // mới thêm
    StairCollider(float x, float y, float w, float h, StairDirection dir, bool top)
        : Collider(x, y, w, h, 0.0f, 0.0f, isTop), direction(dir), isTop(top) 
    {
        shouldBlockThisFrame = top;
        this->isTop = top;
    }

    void SetTemporarilyDisabled(bool value) { temporarilyDisabled = value; }
    bool IsTemporarilyDisabled() const { return temporarilyDisabled; }




};

#endif // STAIRCOLLIDER_H