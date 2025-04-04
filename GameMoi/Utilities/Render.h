#pragma once
#ifndef RENDER_H
#define RENDER_H

#include <d3d11.h>
#include <memory>
#include "SpriteBatch.h"

class Render {
private:
    ID3D11Device* device = nullptr;   
    ID3D11DeviceContext* deviceContext = nullptr;  
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
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }  
};

#endif // RENDER_H
