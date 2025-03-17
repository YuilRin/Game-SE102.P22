#include "Render.h"
#include "WICTextureLoader.h"

bool Render::Init(HWND hwnd, int width, int height) {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;

    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
        D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &deviceContext))) {
        return false;
    }

    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
    backBuffer->Release();
    deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
    deviceContext->RSSetViewports(1, &viewport);

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);

    return true;
}

void Render::BeginRender() {
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
    spriteBatch->Begin();
}

void Render::EndRender() {
    spriteBatch->End();
    swapChain->Present(1, 0);
}

std::unique_ptr<DirectX::SpriteBatch>& Render::GetSpriteBatch() {
    return spriteBatch;
}

void Render::Cleanup() {
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
}
