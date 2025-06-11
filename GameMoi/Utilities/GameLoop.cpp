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
            Info* playerInfo = player->GetInfo();
            player->UpdatePosition(deltaTime);
            if (playerInfo) {
                // Cập nhật heart/HP từ player
                gameUI->SetHeart(playerInfo->GetHeart());

                // Cập nhật các thông tin khác
                gameUI->SetLife(playerInfo->GetLife());
                gameUI->SetScore(playerInfo->GetScore());
                gameUI->SetStage(playerInfo->GetStage());
                gameUI->SetTime(playerInfo->GetTime());
                gameUI->SetMaxWeapon(playerInfo->GetMaxWeapon());

                // Debug: In ra để kiểm tra
                char message[100];
                sprintf_s(message, "Player HP: %d, UI HP: %d",
                    playerInfo->GetHeart(), gameUI->GetPlayerHitPoint());
                // Uncomment dòng dưới để debug
                // MessageBoxA(NULL, message, "Debug HP", MB_OK | MB_ICONINFORMATION);
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


        // Level transition logic
        if (world->GetPlayer()->GetX() >= 255 && world->GetPlayer()->GetY() <= -36) {
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

void LoadNextLevel() {
    ID3D11Device* device = renderer.GetDevice();
    ID3D11DeviceContext* context = renderer.GetDeviceContext();

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

    SceneBuilder::LoadSceneWithData(world.get(), tileMap.get(), level2, device, context);

    // Update UI for new stage
    if (gameUI) {
        gameUI->SetStage(2);
        gameUI->AddScore(1000); // Stage completion bonus
    }
}