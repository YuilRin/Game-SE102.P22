#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800 
#endif

#pragma warning(disable: 4018) // Signed/unsigned mismatch
#pragma warning(disable: 4244) // Data loss in conversion
#pragma warning(disable: 4305) // Truncation from double to float
#pragma warning(disable: 4715) // No return value in non-void function
#pragma warning(disable: 4800) // Forcing value to bool

#ifndef __GAMEUIT_FRAMEWORK__
#define __GAMEUIT_FRAMEWORK__

#define _USE_MATH_DEFINES

// ============================
//  Thay thế DirectX 9 bằng DirectX 11
// ============================
#include <d3d11.h>       // Direct3D 11
#include <d3dcompiler.h> // Shader compiler
#include <DirectXMath.h> // Hỗ trợ toán học 3D (thay thế D3DX)
#include <windows.h>     // Windows API
#include <exception>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include <wrl/client.h>  // Microsoft::WRL::ComPtr (Quản lý bộ nhớ COM)

// Sử dụng DirectXMath
using namespace DirectX;
using namespace std;

// ============================
//  Định nghĩa cửa sổ
// ============================
#define WINDOW_WIDTH  512
#define WINDOW_HEIGHT 450
#define SCALE_FACTOR  1.0f

// ============================
//  Màu sắc
// ============================
#define COLOR_WHITE XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) 
#define COLOR_KEY   XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) 

// ============================
//  Enum định danh đối tượng
// ============================
enum eID
{
    UNKNOWN = -1,
    PLAYER = 0,
    MAIN_MENU = 1,
    FONTFULL = 2,

    INFO = 4,
    HEART_ICON = 5,
    HIT_POINT_ICON = 6,
    BORDER = 7,

    WEAPON = 10,
    DAGGER = 11,
    AXE = 12,
    BOOMERANG = 13,

    MAP_STAGE_21 = 21,
    MAP_STAGE_22 = 22,
    MAP_STAGE_23 = 23,

    BOSS = 200,
    MEDUSA = 201,
    SNAKE = 202,
};

// ============================
//  Trạng thái của đối tượng
// ============================
enum eStatus
{
    NORMAL = 0,
    MOVING_LEFT = (1 << 0),
    MOVING_RIGHT = (1 << 1),
    JUMPING = (1 << 2),
    RUNNING = (1 << 3),
    DIE = (1 << 4),
    ATTACKING = (1 << 8),
};

// ============================
//  Hướng di chuyển
// ============================
enum eDirection
{
    NONE = 0,
    TOP = 1,
    BOTTOM = 2,
    LEFT = 4,
    RIGHT = 8,
    ALL = (TOP | BOTTOM | LEFT | RIGHT),
};

// ============================
//  ID Âm thanh
// ============================
enum eSoundId
{
    INTRO_SCENE, PLAY_SCENE, OVER_SCENE,
    OPEN_DOOR, WIN_LEVEL,
    HIT_SOUND, DIE_SOUND,
    AXE_SOUND, DAGGER_SOUND,
    BOSS_SOUND, TIME_OUT,
    GET_ITEM, GET_MONEY, GET_HEART,
};

// ============================
//  Kiểu vector mới
// ============================
typedef XMFLOAT3 GVector3;
typedef XMFLOAT2 GVector2;

#define VECTOR2ZERO GVector2(0.0f, 0.0f)
#define VECTOR2ONE  GVector2(1.0f, 1.0f)

// ============================
//  SAFE_DELETE cho DirectX 11
// ============================
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = nullptr; } }
#endif

// ============================
//  SAFE_RELEASE dành cho COM object
// ============================
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#endif

#endif // __GAMEUIT_FRAMEWORK__
