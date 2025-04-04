#pragma once
#ifndef __LIFEUI_H__
#define __LIFEUI_H__

#include "StopWatch.h"

#define GAP 10

class Info  // : public BaseObject
{
public:
	Info();
	~Info();

	virtual void init();
	
	//chưa làm phần vẽ cho Info
	//virtual void update(float deltatime);
	//virtual void draw(LPD3DXSPRITE, Viewport*);
	//virtual void release();

	void SetPlayerHitPoint(int number);
	int GetPlayerHitPoint();

	void SetEnemyHitPoint(int number);
	int GetEnemyHitPoint();

	void SetHeart(int number);
	int GetHeart();

	void SetLife(int number);
	int GetLife();

	void SetStage(int number);
	int GetStage();

	void AddScore(int number);
	void SetScore(int number);
	int GetScore();

	void SetTime(int number);
	void PauseTime();
	void ActiveTime();
	int GetTime();

	void SetWeapon(eID id);
	eID GetCurrentWeapon();

	void SetMaxWeapon(int num);
	int GetMaxWeapon();
private:
	
	int _playerHitPointNumber;
	int _enemyHitPointNumber;
	int _heartNumber;
	int _lifeNumber;
	int _scoreNumber;
	int _stageNumber;
	int _timeNumber;

	float _beginTime;
	bool _pauseTime;
	int time;
	int _maxWeapon;


	/*eID _weaponID;
	Sprite* _weaponSprite;
	Sprite* _iconEmptyHitPoint;
	Sprite* _border;

	Sprite* _iconPlayerHitPoint;
	TextSprite* _textPlayer;
	

	Sprite* _iconEnemyHitPoint;
	TextSprite* _textEnemy;
	

	Sprite* _iconHeart;
	TextSprite* _textHeart;
	

	TextSprite* _textLife;

	TextSprite* _textScore;
	

	TextSprite* _textStage;
	

	TextSprite* _textTime;
	
	Sprite* _maxWeaponSprite;*/
};

#endif // !__LIFEUI_H__
