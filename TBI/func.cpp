#include "global.h"
#include "func.h"

//0~255���� -> 0~1�� ����ȭ
D2D1::ColorF ColorNormalize(int r, int g, int b)
{
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
}