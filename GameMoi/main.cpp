#include "Utilities/GameInit.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (!InitGame(hInstance, nCmdShow)) 
        return 0;
    GameLoop();
    return 0;
}
