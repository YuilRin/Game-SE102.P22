#include "WindowProc.h"
#include "GameInit.h"

void HandleInput(WPARAM key) {
    if (world && world->GetPlayer()) {
        world->GetPlayer()->onKeyPressed(key);
    }
    if (world && world->GetPlayer2()) {
        world->GetPlayer2()->onKeyPressed(key);
    }
}

void HandleOutput(WPARAM key) {
    if (world && world->GetPlayer()) {
        world->GetPlayer()->onKeyReleased(key);
    }   
    if (world && world->GetPlayer2()) {
        world->GetPlayer2()->onKeyReleased(key);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_KEYDOWN:
        HandleInput(wParam);
        break;
    case WM_KEYUP:
        HandleOutput(wParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

