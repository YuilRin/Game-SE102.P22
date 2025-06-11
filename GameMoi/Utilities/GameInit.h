#pragma once

#ifndef GAME_INIT_H
#define GAME_INIT_H

#include <memory>
#include "../Render.h"
#include "../Models/World.h" 
#include "../TileMap/TileMap.h"
#include "GameLoop.h"
#include "../Camera/Camera.h"
#include "WICTextureLoader.h"
#include "../Tilemap/Collider.h"
#include "SceneBuilder.h"
#include "LevelData.h"
#include "../Models/Characters/Info.h" 

extern std::unique_ptr<Info> gameUI;
extern std::function<void(int)> OnEnemyKilled;
extern std::function<void(int)> OnItemCollected;
extern HWND hwnd;
extern Render renderer;
extern std::unique_ptr<World> world; 
extern std::unique_ptr<TileMap> tileMap;
extern CCamera* camera;

const int WIDTH = 512;
const int HEIGHT = 450;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool InitGame(HINSTANCE hInstance, int nCmdShow);

#endif // GAME_INIT_H
