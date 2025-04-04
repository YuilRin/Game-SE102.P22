#include "WindowProc.h"
#include "GameInit.h"

void HandleInput(WPARAM key) {
    player->onKeyPressed(key);
}

void HandleOutput(WPARAM key) {
    player->onKeyReleased(key);
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

