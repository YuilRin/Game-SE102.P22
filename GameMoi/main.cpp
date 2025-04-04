#include <windows.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include "Render.h"
#include "Models/Characters/Player.h"
#include "Models/Characters/Enemy.h"
#include "WICTextureLoader.h"
#include "Tilemap/TileMap.h"
#include "Camera/Camera.h"
#include <algorithm>
#include "GameInit.h"

using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (!InitGame(hInstance, nCmdShow)) 
        return 0;

    GameLoop();
    return 0;
}
