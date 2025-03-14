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
#include "CKeyMgr.h"

#include "CScene_Main.h"


CScene_Main::CScene_Main()
	: Cursor(nullptr)
	, m_iCursorPos(0)
{

}

CScene_Main::~CScene_Main()
{

}


void CScene_Main::Enter()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CreateTitle(pD2DMgr, vResolution);

	CreateGameMenu(pD2DMgr, vResolution);

	wstring mainTitleBGMKey = L"genesis_retake_light_loop";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.2f);
	}
}

void CScene_Main::Exit()
{
	DeleteAll();
}

void CScene_Main::update()
{
	if (KEY_TAP(KEY::ENTER) || KEY_TAP(KEY::SPACE))
	{
		Vec2 vPos = CCore::GetInstance()->GetResolution();
		CSoundMgr::GetInstance()->Play(L"book page turn");
		CCamera::GetInstance()->SetLookAt(Vec2(0.f, 540.f));
	}

	if (KEY_TAP(KEY::UP)) {
		if (m_iCursorPos == 0) m_iCursorPos = 2;
		else m_iCursorPos--;
		
		Cursor->SetOffset(CursorPos[m_iCursorPos]);
		CSoundMgr::GetInstance()->Play(L"menu_scroll");
	}
	if (KEY_TAP(KEY::DOWN)) {
		if (m_iCursorPos == 2) m_iCursorPos = 0;
		else m_iCursorPos++;

		Cursor->SetOffset(CursorPos[m_iCursorPos]);
		CSoundMgr::GetInstance()->Play(L"menu_scroll");
	}

	CScene::update();
}

void CScene_Main::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Main::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Main::CreateTitle(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
{
	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"titlemenu_2"), L"title_background",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 270.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"titlemenu_2"), L"title_character",
		D2D1::Point2F(0.f, 405.f), D2D1::Point2F(480.f, 540.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"logo"), L"title_logo_text",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 160.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"logo"), L"title_logo_shadow",
		D2D1::Point2F(0.f, 160.f), D2D1::Point2F(480.f, 320.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"fly"), L"title_fly",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(96.f, 160.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"fly"), L"title_fly_shadow",
		D2D1::Point2F(96.f, 0.f), D2D1::Point2F(192.f, 160.f));

	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* MainPanel = new CSpriteUI;
	MainPanel->SetObjType(GROUP_TYPE::IMAGE);
	MainPanel->SetName(L"title_background");
	MainPanel->AddImage(_pD2DMgr->GetStoredBitmap(L"title_background"));
	MainPanel->SetPos(Vec2(0.f,0.f));
	MainPanel->SetScale(_vResolution);
	AddObject(MainPanel, GROUP_TYPE::IMAGE);
	///////////////////////뒷 배경/////////////////////////

	
	////////////////////////가운데 캐릭터//////////////////////////////
	CSpriteUI* MainCharacter = MainPanel->AddChild<CSpriteUI>(Vec2(-10.f, 60.f));
	MainCharacter->SetObjType(GROUP_TYPE::IMAGE);

	MainCharacter->CreateAnimator();
	MainCharacter->GetAnimator()->CreateAnimation(L"title_character", _pD2DMgr->GetStoredBitmap(L"title_character")
		, Vec2(0.f, 0.f), Vec2(160.f, 135.f), Vec2(160.f, 0.f), 0.4f, 2);
	MainCharacter->GetAnimator()->Play(L"title_character", true);

	MainCharacter->SetName(L"title_character");
	MainCharacter->SetScale(Vec2(160.f, 135.f) * 2.f);
	MainCharacter->SetRenderScale(MainCharacter->GetRenderScale() * 2.f);
	AddObject(MainCharacter, GROUP_TYPE::IMAGE);
	////////////////////////가운데 캐릭터//////////////////////////////


	///////////////////////메인 로고 그림자/////////////////////////
	CSpriteUI* MainLogoShadow = MainPanel->AddChild<CSpriteUI>(Vec2(0.f, -115.f));
	MainLogoShadow->SetMovement(5.f, 5.f);
	MainLogoShadow->SetObjType(GROUP_TYPE::IMAGE);
	MainLogoShadow->SetName(L"title_logo_shadow");
	MainLogoShadow->AddImage(_pD2DMgr->GetStoredBitmap(L"title_logo_shadow"));
	//MainLogoShadow->SetPos(_vResolution / 2.f);
	MainLogoShadow->SetScale(Vec2(480.f, 160.f) * 2);
	AddObject(MainLogoShadow, GROUP_TYPE::IMAGE);
	///////////////////////메인 로고 그림자/////////////////////////

	///////////////////////메인 로고 텍스트/////////////////////////
	CSpriteUI* MainLogoText = MainLogoShadow->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	MainLogoText->SetObjType(GROUP_TYPE::IMAGE);
	MainLogoText->SetName(L"title_logo_text");
	MainLogoText->AddImage(_pD2DMgr->GetStoredBitmap(L"title_logo_text"));
	//MainLogoText->SetPos(_vResolution / 2.f);
	MainLogoText->SetScale(Vec2(480.f, 160.f) * 2);
	AddObject(MainLogoText, GROUP_TYPE::IMAGE);
	///////////////////////메인 로고 텍스트/////////////////////////


	////////////////////////파리////////////////////////////////////
	CSpriteUI* MainFly = MainPanel->AddChild<CSpriteUI>(Vec2(300.f, 0.f));
	MainFly->SetObjType(GROUP_TYPE::IMAGE);
	MainFly->SetName(L"title_fly");
	MainFly->CreateAnimator();

	MainFly->GetAnimator()->CreateAnimation(L"title_fly", _pD2DMgr->GetStoredBitmap(L"title_fly")
		, Vec2(0.f, 0.f), Vec2(96.f, 80.f), Vec2(0.f, 80.f), 0.1f, 2);
	MainFly->GetAnimator()->Play(L"title_fly", true);

	MainFly->SetScale(Vec2(96.f, 80.f) * 2);
	MainFly->SetRenderScale(MainFly->GetScale() * 2.f);
	AddObject(MainFly, GROUP_TYPE::IMAGE);
	////////////////////////파리////////////////////////////////////

	////////////////////////파리 그림자////////////////////////////////////
	CSpriteUI* MainFlyShadow = MainFly->AddChild<CSpriteUI>(Vec2(0.f, 25.f));
	MainFlyShadow->SetObjType(GROUP_TYPE::IMAGE);
	MainFlyShadow->SetName(L"title_fly_shadow");
	MainFlyShadow->CreateAnimator();

	MainFlyShadow->GetAnimator()->CreateAnimation(L"title_fly_shadow", _pD2DMgr->GetStoredBitmap(L"title_fly_shadow")
		, Vec2(0.f, 0.f), Vec2(96.f, 80.f), Vec2(0.f, 80.f), 0.1f, 2);
	MainFlyShadow->GetAnimator()->Play(L"title_fly_shadow", true);

	MainFlyShadow->SetScale(Vec2(96.f, 80.f) * 2);
	MainFlyShadow->SetRenderScale(MainFlyShadow->GetScale() * 2.f);
	AddObject(MainFlyShadow, GROUP_TYPE::IMAGE);
	////////////////////////파리 그림자////////////////////////////////////

	///////////////////////화면 overlay/////////////////////////
	CSpriteUI* MainMenuOverlay = new CSpriteUI;
	MainMenuOverlay->SetObjType(GROUP_TYPE::EFFECT);
	MainMenuOverlay->SetName(L"title_menuoverlay");
	MainMenuOverlay->AddImage(_pD2DMgr->GetStoredBitmap(L"menuoverlay"));
	MainMenuOverlay->SetPos(_vResolution / 2.f);
	MainMenuOverlay->SetScale(_vResolution);
	AddObject(MainMenuOverlay, GROUP_TYPE::EFFECT);
	///////////////////////화면 overlay/////////////////////////

	///////////////////////화면 overlay 그림자/////////////////////////
	CSpriteUI* MainMenuShadow = MainMenuOverlay->AddChild<CSpriteUI>(Vec2(-_vResolution.x / 2.f + 256.f, _vResolution.y / 2.f - 150.f));
	MainMenuShadow->SetObjType(GROUP_TYPE::EFFECT);
	MainMenuShadow->SetName(L"title_menushadow");
	MainMenuShadow->AddImage(_pD2DMgr->GetStoredBitmap(L"menushadow"));
	MainMenuShadow->SetScale(Vec2(256.f,150.f) * 2.f);
	AddObject(MainMenuShadow, GROUP_TYPE::EFFECT);
	///////////////////////화면 overlay 그림자/////////////////////////
}

void CScene_Main::CreateGameMenu(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
{
	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu"), L"gamemenu_panel",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 270.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu"), L"gamemenu_downside",
		D2D1::Point2F(0.f, 271.f), D2D1::Point2F(480.f, 540.f));
	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"cursor",
		D2D1::Point2F(0.f, 30.f), D2D1::Point2F(30.f, 60.f));

	/////////////////////////////////////////////////////////////////////////////////////////
	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"new_challenge",
		D2D1::Point2F(30.f, 20.f), D2D1::Point2F(130.f, 68.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"run_continue",
		D2D1::Point2F(30.f, 20.f + 48.f * 1.f), D2D1::Point2F(130.f, 20.f + 48.f * 2.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"challenge_list",
		D2D1::Point2F(30.f, 20.f + 48.f * 2.f), D2D1::Point2F(130.f, 20.f + 48.f * 3.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"statistics",
		D2D1::Point2F(30.f, 20.f + 48.f * 3.f), D2D1::Point2F(130.f, 20.f + 48.f * 4.f));

	_pD2DMgr->SplitBitmap(_pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"setting",
		D2D1::Point2F(30.f, 20.f + 48.f * 4.f), D2D1::Point2F(130.f, 20.f + 48.f * 5.f));
	/////////////////////////////////////////////////////////////////////////////////////////
	// 
	// 
	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* GameMenuBackGround = new CSpriteUI;
	GameMenuBackGround->SetObjType(GROUP_TYPE::IMAGE);
	GameMenuBackGround->SetName(L"gamemenu_background");
	GameMenuBackGround->AddImage(_pD2DMgr->GetStoredBitmap(L"emptyscreen"));
	GameMenuBackGround->SetPos(Vec2(0.f, 540.f));
	GameMenuBackGround->SetScale(_vResolution);
	AddObject(GameMenuBackGround, GROUP_TYPE::IMAGE);
	///////////////////////뒷 배경/////////////////////////

	///////////////////////게임 메뉴 패널/////////////////////////
	CSpriteUI* GameMenuPanel = new CSpriteUI;
	GameMenuPanel->SetObjType(GROUP_TYPE::IMAGE);
	GameMenuPanel->SetName(L"gamemenu_panel");
	GameMenuPanel->AddImage(_pD2DMgr->GetStoredBitmap(L"gamemenu_panel"));
	GameMenuPanel->SetPos(Vec2(0.f, 540.f));
	GameMenuPanel->SetScale(_vResolution);
	AddObject(GameMenuPanel, GROUP_TYPE::IMAGE);
	///////////////////////게임 메뉴 패널/////////////////////////

	///////////////////////커서/////////////////////////
	Cursor = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(-140.f,-150.f));
	Cursor->SetObjType(GROUP_TYPE::IMAGE);
	Cursor->SetName(L"cursor");
	Cursor->AddImage(_pD2DMgr->GetStoredBitmap(L"cursor"));
	Cursor->SetScale(Vec2(30.f,30.f) * 2.f);
	AddObject(Cursor, GROUP_TYPE::IMAGE);
	///////////////////////커서/////////////////////////

	///////////////////////새 도전 글자/////////////////////////
	CSpriteUI* NewChallenge = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -150.f));
	NewChallenge->SetObjType(GROUP_TYPE::IMAGE);

	//NewChallenge->SetBackGround(true);
	//NewChallenge->SetBackGroundColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	NewChallenge->SetName(L"new_challenge");
	NewChallenge->AddImage(_pD2DMgr->GetStoredBitmap(L"new_challenge"));
	NewChallenge->SetScale(Vec2(100.f, 48.f) * 2);
	AddObject(NewChallenge, GROUP_TYPE::IMAGE);
	///////////////////////새 도전 글자/////////////////////////

	///////////////////////계속하기 글자/////////////////////////
	CSpriteUI* RunContinue = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -90.f));
	RunContinue->SetObjType(GROUP_TYPE::IMAGE);

	//NewChallenge->SetBackGround(true);
	//NewChallenge->SetBackGroundColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	RunContinue->SetName(L"run_continue");
	RunContinue->AddImage(_pD2DMgr->GetStoredBitmap(L"run_continue"));
	RunContinue->SetScale(Vec2(100.f, 48.f) * 2);
	AddObject(RunContinue, GROUP_TYPE::IMAGE);
	///////////////////////계속하기 글자/////////////////////////



	///////////////////////설정 글자/////////////////////////
	CSpriteUI* Setting = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -30.f));
	Setting->SetObjType(GROUP_TYPE::IMAGE);

	//Setting->SetBackGround(true);
	//Setting->SetBackGroundColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	Setting->SetName(L"setting");
	Setting->AddImage(_pD2DMgr->GetStoredBitmap(L"setting"));
	Setting->SetScale(Vec2(100.f, 48.f) * 2);
	AddObject(Setting, GROUP_TYPE::IMAGE);
	///////////////////////설정 글자/////////////////////////
}
