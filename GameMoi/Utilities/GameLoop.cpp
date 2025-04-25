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

        // Rendering
        renderer.BeginRender();
        tileMap->Draw(&renderer, camera);
        if (world) {
            world->Render(renderer.GetSpriteBatch());
        }
        renderer.EndRender();
    }
}
