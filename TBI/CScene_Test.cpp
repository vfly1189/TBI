#include "global.h"

#include "CObject.h"
#include "CPlayer.h"

#include "CSoundMgr.h"
#include "CFontMgr.h"

#include "CScene_Test.h"

CScene_Test::CScene_Test()
{

}

CScene_Test::~CScene_Test()
{

}

void CScene_Test::Enter()
{
	// 시스템에 설치된 폰트 로딩 (예: Segoe UI)
	HRESULT hr = CFontMgr::GetInstance()->LoadSystemFont(L"HY견명조", 24.f);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed to load system font.", L"Error", MB_OK);
	}

	CObject* test = new CPlayer;
	test->SetPos(Vec2(0.f, 0.f));
	test->SetScale(Vec2(100.f, 100.f));
	test->SetUIText(L"1234");
	AddObject(test, GROUP_TYPE::DEFAULT);

	wstring mainTitleBGMKey = L"main_title_bgm";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.4f);
	}
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
