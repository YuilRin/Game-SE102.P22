#include "Info.h"

Info::Info()
{

}

Info::~Info()
{

}

void Info::init()
{
	_playerHitPointNumber = 0;
	_enemyHitPointNumber = 0;
	_heartNumber = 1;
	_lifeNumber = 1;
	_scoreNumber = 1;
	_stageNumber = 1;
	_timeNumber = 1;
}

void Info::SetPlayerHitPoint(int number)
{
	_playerHitPointNumber = number;
	if (_playerHitPointNumber > 16)
		_playerHitPointNumber = 16;
}

int Info::GetPlayerHitPoint()
{
	return _playerHitPointNumber;
}

void Info::SetEnemyHitPoint(int number)
{
	_enemyHitPointNumber = number;
}

int Info::GetEnemyHitPoint()
{
	return _enemyHitPointNumber;
}

void Info::SetHeart(int number)
{
	_heartNumber = number;
	if (_heartNumber >= 100)
		_heartNumber = 99;
}

int Info::GetHeart()
{
	return _heartNumber;
}

void Info::AddHeart(int number)
{
	_heartNumber += number;
	if (_heartNumber >= 100)
		_heartNumber = 99;
}

void Info::SetLife(int number)
{
	_lifeNumber = number;
}

int Info::GetLife()
{
	return _lifeNumber;
}



void Info::SetScore(int number)
{
	_scoreNumber = number;
}





void Info::SetTime(int number)
{
	if (!_pauseTime)
	{
		_timeNumber = number + 1;
		_beginTime = GameTime::getInstance()->getTotalGameTime();
		time = number;
	}
	else
		time = number;
}

void Info::ActiveTime()
{
	_pauseTime = false;
}

void Info::PauseTime()
{
	_pauseTime = true;
}

int Info::GetTime()
{
	return time;
}

void Info::TakeDamage(int damage)
{
	SetHeart(_heartNumber - damage);
}
