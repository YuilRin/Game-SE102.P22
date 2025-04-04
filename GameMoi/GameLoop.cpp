#include "GameLoop.h"
#include "GameInit.h"  // Để sử dụng player, renderer, camera
#include <windows.h>    // Để dùng GetTickCount()

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

        // Cập nhật game
        player->Update(deltaTime);

        float camX = player->GetX() - WIDTH / 3;

        // Giới hạn camera
        float leftEdge = 0;
        float rightEdge = tileMap->GetWidth() - WIDTH / 3;
        camX = max(leftEdge, min(camX, rightEdge));
        camera->SetPosition(camX, 0);

        // Vẽ game
        renderer.BeginRender();
        tileMap->Draw(&renderer, camera);
        player->Render(renderer.GetSpriteBatch());
        renderer.EndRender();
    }
}
