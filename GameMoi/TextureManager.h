
#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include <wrl/client.h>
#include <wincodec.h>
//#include <DirectXTK/SimpleMath.h>

using Microsoft::WRL::ComPtr;

class TextureManager {
private:
    static std::map<std::string, ID3D11ShaderResourceView*> textures;
    static ID3D11Device* device;
    static ID3D11DeviceContext* context;

public:
    static void Init(ID3D11Device* dev, ID3D11DeviceContext* ctx);
    static ID3D11ShaderResourceView* Load(const std::string& id, const std::wstring& filepath);
    static ID3D11ShaderResourceView* Get(const std::string& id);
    static void Release();
};
