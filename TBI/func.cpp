#include "global.h"
#include "func.h"

//0~255값을 -> 0~1로 정규화
D2D1::ColorF ColorNormalize(int r, int g, int b)
{
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
}