//// Sprite.h
//#pragma once
//
//#include <DirectXMath.h>
//#include <d3d11.h>
//#include "Texture.h"
//
//using namespace DirectX;
//
//class Sprite {
//private:
//    int id;
//    Texture* texture;
//
//    XMFLOAT2 texCoord; // top-left UV
//    XMFLOAT2 texSize;  // UV width/height
//
//    int width, height;
//
//public:
//    Sprite(int id, Texture* texture, int left, int top, int right, int bottom);
//
//    void Draw(ID3D11DeviceContext* context, float x, float y);
//
//    int GetID() const { return id; }
//};
