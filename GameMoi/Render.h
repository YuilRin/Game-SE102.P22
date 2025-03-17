#pragma once
#ifndef RENDER_H
#define RENDER_H

#include <d3d11.h>
#include <memory>
#include "SpriteBatch.h"

class Render {
private:
    ID3D11Device* device = nullptr;   // ? Gi? nguy�n v� kh?i t?o lu�n
    ID3D11DeviceContext* deviceContext = nullptr;  // ? S?a t�n bi?n cho ?�ng
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

public:
    bool Init(HWND hwnd, int width, int height);
    void BeginRender();
    void EndRender();
    std::unique_ptr<DirectX::SpriteBatch>& GetSpriteBatch();
    void Cleanup();

    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }  // ? S?a l?i t�n bi?n
};

#endif // RENDER_H
