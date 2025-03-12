#include "global.h"

#include "CObject.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{

}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{

}

void CPlayer::render(ID2D1HwndRenderTarget* _pRender)
{
	CObject::render(_pRender);
}

