#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <vector>
#include "Animation.h"
class Player : public GameObject {
private:
    float speed;

    bool facingLeft;
    float velocityY;

    float _gravity = 9.8f;
    bool _isJumping;
    bool _isAttacking;
    bool isOnGround;
    bool _isDead;
  
	/* Animation leftAnimation; bỏ đi
    Animation rightAnimation;
    Animation* currentAnimation;*/

    Animation animation;


    //std::map<int, Animation*> _animations;
    //std::vector<Weapon*> _listWeapon;

//    CollisionBody* _collisionBody;
  //  StopWatch* _attackStopWatch;

public:
   /* Player(float x, float y,
        const std::vector<ID3D11ShaderResourceView*>& leftTextures,
        const std::vector<ID3D11ShaderResourceView*>& rightTextures,
        float frameTime);*/
    Player(float x, float y,
        ID3D11ShaderResourceView* spriteSheet, 
        const std::vector<Frame>& frames, 
        float frameTime);

    void HandleInput(WPARAM key);
    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    //void moveLeft();
    //void moveRight();
    //void jump();
    //void attack();
    //void die();
    //void revive();

    //// Xử lý va chạm
    //float checkCollision(GameObject* object, float dt);
    //void applyGravity(float dt);

    //bool isAttacking() const;
    //bool isDead() const;
};

#endif

