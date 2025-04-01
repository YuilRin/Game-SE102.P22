#include "Camera.h"
CCamera* CCamera::instance = nullptr;
CCamera* CCamera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CCamera();
	}

	return instance;
}
void CCamera::Init()
{

	x = 0.0f;
	y = 0.0f;
	width = 0;
	height = 0;
}
void CCamera::SetPosition(float x, float y)
{

	this->x = x;
	this->y = y;

}

void CCamera::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}


void CCamera::Reset()
{
	x = 0.0f;
	y = 0.0f;
}


float CCamera::GetTop()
{
	return y;
}

float CCamera::GetLeft()
{
	return x;
}

float CCamera::GetRight()
{
	return x + width;
}

float CCamera::GetBottom()
{
	return y + height;
}

CCamera::~CCamera()
{
	SAFE_DELETE(instance);
}
