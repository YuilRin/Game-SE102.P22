#include "TextureManager.h"
#include <WICTextureLoader.h>

std::map<std::string, ID3D11ShaderResourceView*> TextureManager::textures;
ID3D11Device* TextureManager::device = nullptr;
ID3D11DeviceContext* TextureManager::context = nullptr;

void TextureManager::Init(ID3D11Device* dev, ID3D11DeviceContext* ctx) {
    device = dev;
    context = ctx;
}

ID3D11ShaderResourceView* TextureManager::Load(const std::string& id, const std::wstring& filepath) {
    if (textures.find(id) != textures.end()) return textures[id];

    ID3D11ShaderResourceView* texture = nullptr;
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, filepath.c_str(), nullptr, &texture);
    if (SUCCEEDED(hr)) {
        textures[id] = texture;
        return texture;
    }

    return nullptr;
}

ID3D11ShaderResourceView* TextureManager::Get(const std::string& id) {
    if (textures.find(id) != textures.end()) {
        return textures[id];
    }
    return nullptr;
}

void TextureManager::Release() {
    for (auto& pair : textures) {
        if (pair.second) pair.second->Release();
    }
    textures.clear();
}
