#include "utils.h"

bool isIntersectedInGame(RECT rect1, RECT rect2)
{
	float left = rect1.left - rect2.right;
	float right = rect1.right - rect2.left;

	float top = rect1.top - rect2.bottom;
	float bottom = rect1.bottom - rect2.top;

	if (left < 0 && right > 0 && top > 0 && bottom < 0)
		return true;

	if (left > 0 && right < 0 && top < 0 && bottom > 0)
		return true;

	return false;
}


