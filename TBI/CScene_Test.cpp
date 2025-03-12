#include "global.h"

#include "CObject.h"
#include "CPlayer.h"

#include "CScene_Test.h"

CScene_Test::CScene_Test()
{

}

CScene_Test::~CScene_Test()
{

}

void CScene_Test::Enter()
{
	CObject* test = new CPlayer;
	test->SetPos(Vec2(0.f, 0.f));
	test->SetScale(Vec2(100.f, 100.f));
	AddObject(test, GROUP_TYPE::DEFAULT);
}

void CScene_Test::Exit()
{
	DeleteAll();
}

void CScene_Test::update()
{

}

void CScene_Test::render(ID2D1HwndRenderTarget* _pRender)
{

	CScene::render(_pRender);
}
