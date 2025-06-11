#pragma once
#include "LevelData.h"
#include <vector>
#include <memory>

class LevelManager {
private:
    std::vector<LevelData> levels;
    int currentLevelIndex;
    int maxLevels;

public:
    LevelManager();
    ~LevelManager() = default;

    // Khởi tạo tất cả các level
    void InitializeLevels();

    // Lấy level hiện tại
    const LevelData& GetCurrentLevel() const;

    // Chuyển sang level tiếp theo
    bool NextLevel();

    // Lấy số thứ tự level hiện tại (1-based)
    int GetCurrentLevelNumber() const { return currentLevelIndex + 1; }

    // Kiểm tra xem có còn level nào không
    bool HasNextLevel() const { return currentLevelIndex + 1 < maxLevels; }

    // Reset về level đầu tiên
    void ResetToFirstLevel() { currentLevelIndex = 0; }

    // Lấy tổng số level
    int GetTotalLevels() const { return maxLevels; }

private:
    // Tạo dữ liệu cho từng level cụ thể
    LevelData CreateLevel1();
    LevelData CreateLevel2();
    LevelData CreateLevel3();
    LevelData CreateLevel4();
    LevelData CreateLevel5();
};