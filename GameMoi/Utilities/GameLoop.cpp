#define NOMINMAX 
#include "GameLoop.h"
#include "GameInit.h"
#include <algorithm> 
#include <Windows.h> 

void GameLoop() {
    MSG msg = { 0 };
    DWORD prevTime = GetTickCount();

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DWORD currentTime = GetTickCount();
        float deltaTime = (currentTime - prevTime) / 1000.0f;
        prevTime = currentTime;

        // ===== UPDATE GAME =====
        if (world) {
            world->Update(deltaTime);

        }

        // ===== UPDATE UI =====
        if (gameUI && world && world->GetPlayer()) {
            // FIX: Cập nhật UI trực tiếp từ player data
            Player* player = world->GetPlayer();
            Player2* player2 = world->GetPlayer2();
            Info* playerInfo = player->GetInfo();
            player->UpdatePosition(deltaTime);
            player2->UpdatePosition(deltaTime);
            if (playerInfo) {
                // Cập nhật heart/HP từ player
                gameUI->SetHeart(playerInfo->GetHeart());

                // Cập nhật các thông tin khác
                gameUI->SetLife(playerInfo->GetLife());
                gameUI->SetScore(playerInfo->GetScore());
                gameUI->SetStage(levelManager->GetCurrentLevelNumber()); // Cập nhật stage từ LevelManager
                gameUI->SetTime(playerInfo->GetTime());
                gameUI->SetMaxWeapon(playerInfo->GetMaxWeapon());
            }

            // Update UI time, score, etc.
            gameUI->update(deltaTime);

            // Check for boss/enemy health bar
            CheckEnemyHealthBar();
        }

        if (world && world->GetPlayer()) {
            float playerX = world->GetPlayer()->GetWX();  // Tọa độ thế giới
            float playerY = world->GetPlayer()->GetWY();

            float halfScreenWidth = camera->GetWidth() / 2.0f;
            float halfScreenHeight = camera->GetHeight() / 2.0f;

            float mapWidthInPixel = tileMap->GetWidth() * 16;
            float mapHeightInPixel = tileMap->GetHeight() * 16;

            // ✅ Đặt camera sao cho player nằm giữa màn hình
            float camX = playerX - halfScreenWidth;
            float camY = playerY - halfScreenHeight;

            // ✅ Giới hạn camera trong map
            camX = std::max(0.0f, std::min(camX, mapWidthInPixel - camera->GetWidth()));
            camY = std::max(0.0f, std::min(camY, mapHeightInPixel - camera->GetHeight()));

            camera->SetPosition(camX, camY);
        }

        // Level transition logic - sử dụng điều kiện chung hơn
        if (CheckLevelCompleteCondition()) {
            LoadNextLevel();
        }

        // ===== RENDERING =====
        renderer.BeginRender();

        // Render game world
        tileMap->Draw(&renderer, camera);
        if (world) {
            world->Render(renderer.GetSpriteBatch());
        }

        // Render UI (always last to appear on top)
        if (gameUI) {
            gameUI->draw(renderer.GetSpriteBatch(), renderer.GetDeviceContext());
        }

        renderer.EndRender();
    }
}

// ===== HELPER FUNCTIONS =====
void CheckEnemyHealthBar() {
    if (!gameUI || !world) return;

    auto enemies = world->GetEnemies();
    bool hasBoss = false;

    // Check if there's a boss (enemy with high HP)
    for (auto* enemy : enemies) {
        if (enemy && enemy->GetInfo() && enemy->GetInfo()->GetHeart() > 16) {
            // This is a boss
            hasBoss = true;
            gameUI->SetMaxEnemyHP(32); // Assume boss max HP is 32
            gameUI->SetEnemyHitPoint(enemy->GetInfo()->GetHeart());
            gameUI->ShowEnemyBar(true);
            break;
        }
    }

    if (!hasBoss) {
        gameUI->ShowEnemyBar(false);
    }
}

bool CheckLevelCompleteCondition() {
    if (!world || !world->GetPlayer()) return false;

    // Điều kiện hoàn thành level - có thể thay đổi theo từng level
    // Ví dụ: player đến vị trí cuối level hoặc tiêu diệt hết enemy
    Player* player = world->GetPlayer();

    // Điều kiện 1: Player đến vị trí cuối level (như cũ)
    if (player->GetX() >= 255 && player->GetY() <= -36) {
        return true;
    }

    // Điều kiện 2: Tiêu diệt hết enemy (cho boss level)
  /*  auto enemies = world->GetEnemies();
    if (enemies.empty()) {
        return true;
    }*/

    // Có thể thêm các điều kiện khác tùy theo level

    return false;
}

void LoadNextLevel() {
    if (!levelManager) return;

    // Kiểm tra xem còn level nào không
    if (!levelManager->HasNextLevel()) {
        // Hết game - hiển thị thông báo hoàn thành
        MessageBoxA(NULL, "Congratulations! You have completed all levels!",
            "Game Complete", MB_OK | MB_ICONINFORMATION);
        return;
    }

    // Chuyển sang level tiếp theo
    levelManager->NextLevel();

    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

    // Load level mới từ LevelManager
    const LevelData& nextLevel = levelManager->GetCurrentLevel();

    if (SceneBuilder::LoadSceneWithData(world.get(), tileMap.get(), nextLevel, device, context)) {
        // Update UI for new stage
        if (gameUI) {
            gameUI->SetStage(levelManager->GetCurrentLevelNumber());
            gameUI->AddScore(1000); // Stage completion bonus

            // Thông báo chuyển level
            char message[100];
            sprintf_s(message, "Level %d - Start!", levelManager->GetCurrentLevelNumber());
            // Có thể hiển thị thông báo này trên UI thay vì MessageBox
            // MessageBoxA(NULL, message, "New Level", MB_OK | MB_ICONINFORMATION);
        }
    }
}