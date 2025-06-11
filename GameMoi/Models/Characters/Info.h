#pragma once
#ifndef __LIFEUI_H__
#define __LIFEUI_H__

#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <string>
#include <memory>

#define GAP 10

class Info
{
public:
    Info();
    ~Info();

    virtual void init();

    // Render functions
    virtual void update(float deltatime);
    virtual void draw(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, ID3D11DeviceContext* context);
    virtual void release();

    // Load textures
    void LoadTextures(ID3D11Device* device);

    // Player/Enemy HP functions
    void SetPlayerHitPoint(int number);
    int GetPlayerHitPoint();
    void SetEnemyHitPoint(int number);
    int GetEnemyHitPoint();

    // Heart functions
    void SetHeart(int number);
    int GetHeart();
    void AddHeart(int number);

    // Life functions
    void SetLife(int number);
    int GetLife();

    // Stage functions
    void SetStage(int number);
    int GetStage();

    // Score functions
    void AddScore(int number);
    void SetScore(int number);
    int GetScore();

    // Time functions
    void SetTime(int number);
    void PauseTime();
    void ActiveTime();
    int GetTime();

    // Damage function
    void TakeDamage(int damage);

    // Weapon functions
    void SetMaxWeapon(int num);
    int GetMaxWeapon();

    // Enemy/Boss functions
    void SetMaxEnemyHP(int maxHP);
    void ShowEnemyBar(bool show);

    // Update from player
    void UpdateFromPlayer(class Player* player);

private:
    // Game data
    int _playerHitPointNumber = 0;
    int _enemyHitPointNumber = 0;
    int _maxEnemyHP = 16;           // HP tối đa của enemy/boss
    bool _showEnemyBar = false;     // Có hiển thị thanh máu enemy không
    int _heartNumber;
    int _lifeNumber;
    int _scoreNumber;
    int _stageNumber;
    int _timeNumber;

    float _beginTime;
    bool _pauseTime;
    int time;
    int _maxWeapon;

    // Rendering resources
    ID3D11ShaderResourceView* _uiTexture;    // Texture cho UI elements
    ID3D11ShaderResourceView* _fontTexture;  // Texture cho font
    ID3D11ShaderResourceView* _heartTexture;    // Texture cho UI elements
    ID3D11ShaderResourceView* _weaponTexture;  // Texture cho font
    DirectX::XMFLOAT2 _uiPosition;          // Vị trí UI trên màn hình

    // Private render functions
    void DrawHealthBar(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch,
        float x, float y, int currentHP, int maxHP, bool isPlayer);
    void DrawText(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, const std::string& text, float x, float y, DirectX::XMVECTOR color, float scale);
    int GetCharIndex(char c);
    void DrawTextWhite(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, const std::string& text, float x, float y);
    void DrawTextYellow(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, const std::string& text, float x, float y);
    void DrawTextGreen(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, const std::string& text, float x, float y);
    void DrawWeaponInfo(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y);
    void DrawHearts(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y);
    void DrawLifeIcon(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch, float x, float y);

    // Utility function
    std::string FormatNumber(int number, int width);
};

#endif // !__LIFEUI_H__