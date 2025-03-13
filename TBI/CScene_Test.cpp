#include "global.h"

#include "CObject.h"
#include "CPlayer.h"

#include "CUI.h"
#include "CSpriteUI.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CImage.h"

#include "CCore.h"
#include "CSoundMgr.h"
#include "CFontMgr.h"
#include "Direct2DMgr.h"

#include "CScene_Test.h"

CScene_Test::CScene_Test()
{

}

CScene_Test::~CScene_Test()
{

}

void CScene_Test::Enter()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CreateMain(pD2DMgr, vResolution);

	wstring mainTitleBGMKey = L"genesis_retake_light_loop";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.2f);
	}
}

void CScene_Test::Exit()
{
	DeleteAll();
}

void CScene_Test::update()
{
	CScene::update();
}

void CScene_Test::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Test::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Test::CreateMain(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
{
	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"titlemenu_2"), L"main_background",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 270.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"titlemenu_2"), L"main_character",
		D2D1::Point2F(0.f, 405.f), D2D1::Point2F(480.f, 540.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"logo"), L"logoText",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 160.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"logo"), L"logoShadow",
		D2D1::Point2F(0.f, 160.f), D2D1::Point2F(480.f, 320.f));


	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* MainPanel = new CSpriteUI;
	MainPanel->SetObjType(GROUP_TYPE::DEFAULT);
	MainPanel->SetName(L"main_background");
	MainPanel->AddImage(_pD2DMgr->GetStoredBitmap(L"main_background"));
	MainPanel->SetPos(_vResolution / 2.f);
	MainPanel->SetScale(_vResolution);
	AddObject(MainPanel, GROUP_TYPE::DEFAULT);
	///////////////////////뒷 배경/////////////////////////

	////////////////////////가운데 캐릭터//////////////////////////////
	CSpriteUI* test = MainPanel->AddChild<CSpriteUI>(Vec2(-10.f, 60.f));
	test->SetObjType(GROUP_TYPE::DEFAULT);
	test->CreateAnimator();

	//test->SetBackGround(true);
	//test->SetBackGroundColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));

	test->GetAnimator()->CreateAnimation(L"main_character_logo", _pD2DMgr->GetStoredBitmap(L"main_character")
		, Vec2(0.f, 0.f), Vec2(160.f, 135.f), Vec2(160.f, 0.f), 0.4f, 2);
	test->GetAnimator()->Play(L"main_character_logo", true);

	test->SetName(L"main_character");
	test->SetScale(Vec2(160.f, 135.f) * 2.f);
	test->SetRenderScale(test->GetRenderScale() * 2.f);
	AddObject(test, GROUP_TYPE::DEFAULT);
	////////////////////////가운데 캐릭터//////////////////////////////


	///////////////////////메인 로고 그림자/////////////////////////
	CSpriteUI* MainLogoShadow = MainPanel->AddChild<CSpriteUI>(Vec2(0.f, -115.f));
	MainLogoShadow->SetMovement(5.f, 5.f);
	MainLogoShadow->SetObjType(GROUP_TYPE::DEFAULT);
	MainLogoShadow->SetName(L"main_logo");
	MainLogoShadow->AddImage(_pD2DMgr->GetStoredBitmap(L"logoShadow"));
	//MainLogoShadow->SetPos(_vResolution / 2.f);
	MainLogoShadow->SetScale(Vec2(480.f, 160.f) * 2);
	AddObject(MainLogoShadow, GROUP_TYPE::DEFAULT);
	///////////////////////메인 로고 그림자/////////////////////////
	
	///////////////////////메인 로고 텍스트/////////////////////////
	CSpriteUI* MainLogoText = MainLogoShadow->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	MainLogoText->SetObjType(GROUP_TYPE::DEFAULT);
	MainLogoText->SetName(L"main_logo");
	MainLogoText->AddImage(_pD2DMgr->GetStoredBitmap(L"logoText"));
	//MainLogoText->SetPos(_vResolution / 2.f);
	MainLogoText->SetScale(Vec2(480.f, 160.f) * 2);
	AddObject(MainLogoText, GROUP_TYPE::DEFAULT);
	///////////////////////메인 로고 텍스트/////////////////////////

	///////////////////////화면 overlay/////////////////////////
	CSpriteUI* MainMenuOverlay = MainPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	MainMenuOverlay->SetObjType(GROUP_TYPE::DEFAULT);
	MainMenuOverlay->SetName(L"main_menuoverlay");
	MainMenuOverlay->AddImage(_pD2DMgr->GetStoredBitmap(L"menuoverlay"));
	MainMenuOverlay->SetPos(_vResolution / 2.f);
	MainMenuOverlay->SetScale(_vResolution);
	AddObject(MainMenuOverlay, GROUP_TYPE::DEFAULT);
	///////////////////////화면 overlay/////////////////////////

	///////////////////////화면 overlay 그림자/////////////////////////
	CSpriteUI* MainMenuShadow = MainPanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	MainMenuShadow->SetObjType(GROUP_TYPE::DEFAULT);
	MainMenuShadow->SetName(L"main_menushadow");
	MainMenuShadow->AddImage(_pD2DMgr->GetStoredBitmap(L"menushadow"));
	MainMenuShadow->SetPos(_vResolution / 2.f);
	MainMenuShadow->SetScale(_vResolution);
	AddObject(MainMenuShadow, GROUP_TYPE::DEFAULT);
	///////////////////////화면 overlay 그림자/////////////////////////
}
