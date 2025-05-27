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

        // Update game through World
        if (world) {
            world->Update(deltaTime);
        }

        // Camera logic - SỬA DÒNG NÀY
        if (world && world->GetPlayer()) {
            float camX = world->GetPlayer()->GetX() - WIDTH / 3.0f;
            camX = std::max(0.0f, std::min(camX,
                static_cast<float>(tileMap->GetWidth()) - WIDTH / 3.0f));
            camera->SetPosition(camX, 0);
        }
        if (world->GetPlayer()->GetX() >= 255&&world->GetPlayer()->GetY()<=-36)
        {
            ID3D11Device* device = renderer.GetDevice();
            ID3D11DeviceContext* context = renderer.GetDeviceContext();

            LevelData level1;
            level1.mapFile = "Image/frame3.txt";
            level1.tileTexture = L"Image/frame3tileset.png";
            level1.playerTexture = L"Image/simon.png";
            level1.itemTexture = L"Image/items.png";
            level1.enemyTexture = L"Image/zombie.png";
            level1.breakableItemTexture = L"Image/objects.png";

            // Enemy positions
            level1.enemyPositions = {
                {200.0f, 300.0f}
            };

            // Item positions
            level1.itemPositions = {
                {300.0f, 250.0f, ItemType::SMALL_HEART}
            };

            level1.breakableItemPositions = {
                {700.0f, 200.0f, BreakableItemType::STAIR},

                {400.0f, 350.0f, BreakableItemType::BIG_CANDLE1},
                {300.0f, 350.0f, BreakableItemType::BIG_CANDLE1}
            };
            !SceneBuilder::LoadSceneWithData(world.get(), tileMap.get(), level1, device, context);
             
        }
        // Rendering
        renderer.BeginRender();
        tileMap->Draw(&renderer, camera);
        if (world) {
            world->Render(renderer.GetSpriteBatch());
        }
        renderer.EndRender();
    }
}
