#pragma once
#ifndef GAME_INIT_H
#define GAME_INIT_H

#include <memory>
#include "../Render.h"
#include "../Models/Characters/Player.h"
#include "../TileMap/TileMap.h"
#include "GameLoop.h"
#include "../Camera/Camera.h"
#include "WICTextureLoader.h"
#include "../Tilemap/Collider.h"

extern HWND hwnd;
extern Render renderer;
extern std::unique_ptr<Player> player;
extern std::unique_ptr<TileMap> tileMap;
extern CCamera* camera;

const int WIDTH = 800;
const int HEIGHT = 600;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool InitGame(HINSTANCE hInstance, int nCmdShow);

#endif // GAME_INIT_H