//// Sprite.cpp
//#include "Sprite.h"
//#include "Renderer.h"
//#include "Camera.h"
//
//Sprite::Sprite(int id, Texture* texture, int left, int top, int right, int bottom)
//    : id(id), texture(texture)
//{
//    int texWidth = texture->GetWidth();
//    int texHeight = texture->GetHeight();
//
//    width = right - left + 1;
//    height = bottom - top + 1;
//
//    texCoord = {
//        static_cast<float>(left) / texWidth,
//        static_cast<float>(top) / texHeight
//    };
//
//    texSize = {
//        static_cast<float>(width) / texWidth,
//        static_cast<float>(height) / texHeight
//    };
//}
//
//void Sprite::Draw(ID3D11DeviceContext* context, float x, float y)
//{
//    XMFLOAT2 camPos = Camera::GetInstance()->GetPosition();
//    float screenX = x - camPos.x;
//    float screenY = y - camPos.y;
//
//    Renderer::GetInstance()->DrawTexturedQuad(
//        context, texture, screenX, screenY, width, height,
//        texCoord, texSize
//    );
//}
