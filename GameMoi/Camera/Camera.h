#pragma once
#define SAFE_DELETE(pPtr) { delete pPtr; pPtr = nullptr; };
class CCamera
{
public:
	static CCamera* GetInstance();
	void Init();
	void SetPosition(float x, float y);
	void SetSize(int width, int height);

	void Reset();

	float GetTop();
	float GetLeft();

	float GetRight();
	float GetBottom();

	float GetWidth();
	float GetHeight();
	~CCamera();

protected:

	float x;
	float y;

	int width;
	int height;

	static CCamera* instance;

};
