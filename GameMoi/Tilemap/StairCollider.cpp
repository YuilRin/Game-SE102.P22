// StairCollider.cpp
#include <cmath>
#include "StairCollider.h"

void StairCollider::SetFrameBlocking(bool shouldBlock) {
    shouldBlockThisFrame = shouldBlock;
}

void StairCollider::ResetFrameBlocking() {
    // Reset về trạng thái mặc định: top tiles blocking, normal tiles non-blocking
    shouldBlockThisFrame = isTop;
}

bool StairCollider::IsBlocking() const {
    // Override base class method để sử dụng shouldBlockThisFrame
    return shouldBlockThisFrame;
}

void StairCollider::SetBlocking(bool shouldBlock) {
    // Cập nhật cả base class property và frame-specific property
    isBlocking = shouldBlock;
    shouldBlockThisFrame = shouldBlock;
}