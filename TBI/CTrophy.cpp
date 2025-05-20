#include "global.h"
#include "CTrophy.h"

CTrophy::CTrophy()
{
}

CTrophy::~CTrophy()
{
}

void CTrophy::start()
{

}

void CTrophy::update()
{
	CObject::update();
}

void CTrophy::render(ID2D1HwndRenderTarget* _pRender)
{
	CObject::render(_pRender);
}

void CTrophy::OnCollisionEnter(CCollider* _pOther)
{
	ChangeScene(SCENE_TYPE::MAIN);
}

void CTrophy::OnCollision(CCollider* _pOther)
{
}

void CTrophy::OnCollisionExit(CCollider* _pOther)
{
}


