#include "global.h"

#include "CObject.h"
#include "CPlayer.h"

#include "CUI.h"
#include "CSpriteUI.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CImage.h"

#include "CCore.h"
#include "CSoundMgr.h"
#include "CFontMgr.h"
#include "Direct2DMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CKeyMgr.h"
#include "CPlayerMgr.h"

#include "CScene_Main.h"
#include "CScene_Fight.h"


CScene_Main::CScene_Main()
	: Cursor(nullptr)
	, m_iCursorPos(0)
	, m_iCurPage(0)
	, m_bPageControl(false)
	, m_iCurCharacterIndex(0)
	, m_CurShowingCharacter(nullptr)
	, m_bChangeSceneFlag(false)
	, m_fAccTime(0.f)
	, m_fFadeDuration(0.8f)
{

}

CScene_Main::~CScene_Main()
{

}


void CScene_Main::Enter()
{
	m_bChangeSceneFlag = false;
	CSoundMgr::GetInstance()->StopAllSound();
	m_iCurPage = 0;
	CCamera::GetInstance()->SetLookAt(Vec2(0.f,0.f));
	CPlayerMgr::GetInstance()->CreateAndSettingPlayer();
	m_fAccTime = 0.f;
	m_fFadeAlpha = 0.f;
	CItemMgr::GetInstance()->ResetPickUp();

 	pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CreateTitle(vResolution);

	CreateGameMenu(vResolution);

	CreateSelectCharacter(vResolution);

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
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	ChangePage();

	if (KEY_TAP(KEY::ENTER))
	{
		if (m_iCurPage == 2)
		{
			CreateLoadImage(CCore::GetInstance()->GetResolution());

			CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();

			player->SetPlayerStat(vecCharacterInfo[m_iCurCharacterIndex].m_stat);
			float maxVelocity = vecCharacterInfo[m_iCurCharacterIndex].m_stat.m_fMoveSpd;
			player->GetRigidBody()->SetMaxVelocity(Vec2(maxVelocity, maxVelocity));
			player->SetCharacterName(vecCharacterInfo[m_iCurCharacterIndex].m_characterName);
			player->SetCharacterIdx(m_iCurCharacterIndex);

			CPlayerMgr::GetInstance()->SettingImageAndAnimations(m_iCurCharacterIndex);

			CSoundMgr::GetInstance()->Stop(L"genesis_retake_light_loop");
			CSoundMgr::GetInstance()->Play(L"game_start");
			m_bChangeSceneFlag = true;
		}
		else
			printf("여긴데\n");
	}
	if (m_bChangeSceneFlag)
	{
		m_fAccTime += fDT;

		float fDelta;

		if (m_fAccTime < 1.f) fDelta = 1.f;
		else fDelta = -1.f;

		m_fFadeAlpha += (1.f / m_fFadeDuration) * fDelta * fDT;
		m_fFadeAlpha = max(0.f, min(m_fFadeAlpha, 1.f));

		veil->SetBackGroundColor(D2D1::ColorF(0.f, 0.f, 0.f, m_fFadeAlpha), D2D1::ColorF(0.f, 0.f, 0.f, m_fFadeAlpha)
			, D2D1::ColorF(0.f, 0.f, 0.f, m_fFadeAlpha), D2D1::ColorF(0.f, 0.f, 0.f, m_fFadeAlpha));
		CSpriteUI* child = (CSpriteUI*)veil->GetChildsUI()[0];

		child->GetAnimator()->FindAnimation(L"loadImage")->SetAlpha(m_fFadeAlpha);

		printf("시간 :%f\n", m_fAccTime);
		if (m_fAccTime > 1.f)
		{
			printf("씬 전환\n");
			ChangeScene(SCENE_TYPE::FIGHT);
		}
	}


	if (KEY_TAP(KEY::UP)) 
	{
		MoveCursor(-1);
	}
	if (KEY_TAP(KEY::DOWN)) 
	{
		MoveCursor(1);
	}
	
	if (KEY_TAP(KEY::LEFT))
	{
		ChangeCharacter(-1);
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		ChangeCharacter(1);
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

void CScene_Main::ChangePage()
{
	if ((KEY_TAP(KEY::ENTER) || KEY_TAP(KEY::SPACE)) && !m_bPageControl)
	{
		if (m_iCurPage == 1)
		{
			if (m_iCursorPos == 0)
			{
				m_iCurPage = 2;
			}
			else
			{

			}
		}
		else
		{
			m_iCurPage++;
			m_iCurPage = min(m_iCurPage, 2); // 최대 2페이지까지
		}

		CCamera::GetInstance()->SetLookAt(MainCameraPos[m_iCurPage]);
		CSoundMgr::GetInstance()->Play(L"book page turn");

		m_bPageControl = true;
	}
	if ((KEY_AWAY(KEY::ENTER) || KEY_AWAY(KEY::SPACE)))
	{
		m_bPageControl = false;
	}


	if (KEY_TAP(KEY::ESC) && !m_bPageControl)
	{
		CSoundMgr::GetInstance()->Play(L"book page turn");

		// 페이지 전환 로직 간소화
		m_iCurPage--;
		m_iCurPage = max(m_iCurPage, 0); // 최대 2페이지까지
		CCamera::GetInstance()->SetLookAt(MainCameraPos[m_iCurPage]);

		m_bPageControl = true;
	}
	if (KEY_AWAY(KEY::ESC))
	{
		m_bPageControl = false;
	}
}

void CScene_Main::CreateTitle(Vec2 _vResolution)
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"titlemenu_2"), L"title_background",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 270.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"titlemenu_2"), L"title_character",
		D2D1::Point2F(0.f, 405.f), D2D1::Point2F(480.f, 540.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"logo"), L"title_logo_text",
	D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 160.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"logo"), L"title_logo_shadow",
	D2D1::Point2F(0.f, 160.f), D2D1::Point2F(480.f, 320.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"fly"), L"title_fly",
	D2D1::Point2F(0.f, 0.f), D2D1::Point2F(96.f, 160.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"fly"), L"title_fly_shadow",
		D2D1::Point2F(96.f, 0.f), D2D1::Point2F(192.f, 160.f));

	
	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* MainPanel = new CSpriteUI;
	MainPanel->SetObjType(GROUP_TYPE::IMAGE);
	MainPanel->SetName(L"title_background");
	MainPanel->AddImage(pD2DMgr->GetStoredBitmap(L"title_background"));
	MainPanel->SetPos(Vec2(0.f,0.f));
	MainPanel->SetScale(_vResolution);
	AddObject(MainPanel, GROUP_TYPE::IMAGE);
	///////////////////////뒷 배경/////////////////////////
	
	
	////////////////////////가운데 캐릭터//////////////////////////////
	CSpriteUI* MainCharacter = MainPanel->AddChild<CSpriteUI>(Vec2(-10.f, 60.f));
	MainCharacter->SetObjType(GROUP_TYPE::IMAGE);
	MainCharacter->SetName(L"title_character");
	MainCharacter->CreateAnimator();
	MainCharacter->GetAnimator()->CreateAnimation(L"title_character", pD2DMgr->GetStoredBitmap(L"title_character")
		, Vec2(0.f, 0.f), Vec2(160.f, 135.f), Vec2(160.f, 0.f), 0.4f, 2, true, Vec2(0.f, 0.f));
	MainCharacter->GetAnimator()->Play(L"title_character", true, 1);
	MainCharacter->SetScale(Vec2(160.f, 135.f) * 2.f);
	MainCharacter->SetRenderScale(MainCharacter->GetRenderScale() * 2.f);
	////////////////////////가운데 캐릭터//////////////////////////////
	
	
	
	///////////////////////메인 로고 그림자/////////////////////////
	CSpriteUI* MainLogoShadow = MainPanel->AddChild<CSpriteUI>(Vec2(0.f, -115.f));
	MainLogoShadow->SetMovement(5.f, 5.f);
	MainLogoShadow->SetObjType(GROUP_TYPE::IMAGE);
	MainLogoShadow->SetName(L"title_logo_shadow");
	MainLogoShadow->AddImage(pD2DMgr->GetStoredBitmap(L"title_logo_shadow"));
	//MainLogoShadow->SetPos(_vResolution / 2.f);
	MainLogoShadow->SetScale(Vec2(480.f, 160.f) * 2);
	
	///////////////////////메인 로고 그림자/////////////////////////

	///////////////////////메인 로고 텍스트/////////////////////////
	CSpriteUI* MainLogoText = MainLogoShadow->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	MainLogoText->SetObjType(GROUP_TYPE::IMAGE);
	MainLogoText->SetName(L"title_logo_text");
	MainLogoText->AddImage(pD2DMgr->GetStoredBitmap(L"title_logo_text"));
	//MainLogoText->SetPos(_vResolution / 2.f);
	MainLogoText->SetScale(Vec2(480.f, 160.f) * 2);
	
	///////////////////////메인 로고 텍스트/////////////////////////


	////////////////////////파리////////////////////////////////////
	CSpriteUI* MainFly = MainPanel->AddChild<CSpriteUI>(Vec2(300.f, 0.f));
	MainFly->SetObjType(GROUP_TYPE::IMAGE);
	MainFly->SetName(L"title_fly");
	MainFly->CreateAnimator();

	MainFly->GetAnimator()->CreateAnimation(L"title_fly", pD2DMgr->GetStoredBitmap(L"title_fly")
		, Vec2(0.f, 0.f), Vec2(96.f, 80.f), Vec2(0.f, 80.f), 0.1f, 2, true, Vec2(0.f, 0.f));
	MainFly->GetAnimator()->Play(L"title_fly", true, 1);

	MainFly->SetScale(Vec2(96.f, 80.f) * 2);
	MainFly->SetRenderScale(MainFly->GetScale() * 2.f);
	
	////////////////////////파리////////////////////////////////////

	////////////////////////파리 그림자////////////////////////////////////
	CSpriteUI* MainFlyShadow = MainFly->AddChild<CSpriteUI>(Vec2(0.f, 25.f));
	MainFlyShadow->SetObjType(GROUP_TYPE::IMAGE);
	MainFlyShadow->SetName(L"title_fly_shadow");
	MainFlyShadow->CreateAnimator();

	MainFlyShadow->GetAnimator()->CreateAnimation(L"title_fly_shadow", pD2DMgr->GetStoredBitmap(L"title_fly_shadow")
		, Vec2(0.f, 0.f), Vec2(96.f, 80.f), Vec2(0.f, 80.f), 0.1f, 2, true, Vec2(0.f, 0.f));
	MainFlyShadow->GetAnimator()->Play(L"title_fly_shadow", true, 1);

	MainFlyShadow->SetScale(Vec2(96.f, 80.f) * 2);
	MainFlyShadow->SetRenderScale(MainFlyShadow->GetScale() * 2.f);
	
	////////////////////////파리 그림자////////////////////////////////////

	///////////////////////화면 overlay/////////////////////////
	CSpriteUI* MainMenuOverlay = new CSpriteUI;
	MainMenuOverlay->SetObjType(GROUP_TYPE::EFFECT);
	MainMenuOverlay->SetName(L"title_menuoverlay");
	MainMenuOverlay->AddImage(pD2DMgr->GetStoredBitmap(L"menuoverlay"));
	MainMenuOverlay->SetPos(_vResolution / 2.f);
	MainMenuOverlay->SetScale(_vResolution);
	AddObject(MainMenuOverlay, GROUP_TYPE::EFFECT);
	///////////////////////화면 overlay/////////////////////////

	///////////////////////화면 overlay 그림자/////////////////////////
	CSpriteUI* MainMenuShadow = MainMenuOverlay->AddChild<CSpriteUI>(Vec2(-_vResolution.x / 2.f + 256.f, _vResolution.y / 2.f - 150.f));
	MainMenuShadow->SetObjType(GROUP_TYPE::EFFECT);
	MainMenuShadow->SetName(L"title_menushadow");
	MainMenuShadow->AddImage(pD2DMgr->GetStoredBitmap(L"menushadow"));
	MainMenuShadow->SetScale(Vec2(256.f,150.f) * 2.f);
	
	///////////////////////화면 overlay 그림자/////////////////////////
	
}

void CScene_Main::CreateGameMenu(Vec2 _vResolution)
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu"), L"gamemenu_panel",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 270.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu"), L"gamemenu_downside",
		D2D1::Point2F(0.f, 271.f), D2D1::Point2F(480.f, 540.f));
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"cursor",
		D2D1::Point2F(0.f, 30.f), D2D1::Point2F(30.f, 60.f));

	/////////////////////////////////////////////////////////////////////////////////////////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"new_challenge",
		D2D1::Point2F(30.f, 20.f), D2D1::Point2F(130.f, 68.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"run_continue",
		D2D1::Point2F(30.f, 20.f + 48.f * 1.f), D2D1::Point2F(130.f, 20.f + 48.f * 2.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"challenge_list",
		D2D1::Point2F(30.f, 20.f + 48.f * 2.f), D2D1::Point2F(130.f, 20.f + 48.f * 3.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"statistics",
		D2D1::Point2F(30.f, 20.f + 48.f * 3.f), D2D1::Point2F(130.f, 20.f + 48.f * 4.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"gamemenu_downside"), L"setting",
		D2D1::Point2F(30.f, 20.f + 48.f * 4.f), D2D1::Point2F(130.f, 20.f + 48.f * 5.f));
	/////////////////////////////////////////////////////////////////////////////////////////
	// 
	// 
	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* GameMenuBackGround = new CSpriteUI;
	GameMenuBackGround->SetObjType(GROUP_TYPE::IMAGE);
	GameMenuBackGround->SetName(L"gamemenu_background");
	GameMenuBackGround->AddImage(pD2DMgr->GetStoredBitmap(L"emptyscreen"));
	GameMenuBackGround->SetPos(Vec2(0.f, 540.f));
	GameMenuBackGround->SetScale(_vResolution);
	AddObject(GameMenuBackGround, GROUP_TYPE::IMAGE);
	///////////////////////뒷 배경/////////////////////////

	///////////////////////게임 메뉴 패널/////////////////////////
	CSpriteUI* GameMenuPanel = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	GameMenuPanel->SetObjType(GROUP_TYPE::IMAGE);
	GameMenuPanel->SetName(L"gamemenu_panel");
	GameMenuPanel->AddImage(pD2DMgr->GetStoredBitmap(L"gamemenu_panel"));
	GameMenuPanel->SetPos(Vec2(0.f, 540.f));
	GameMenuPanel->SetScale(_vResolution);
	
	///////////////////////게임 메뉴 패널/////////////////////////

	///////////////////////커서/////////////////////////
	Cursor = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(-140.f,-150.f));
	Cursor->SetObjType(GROUP_TYPE::IMAGE);
	Cursor->SetName(L"cursor");
	Cursor->AddImage(pD2DMgr->GetStoredBitmap(L"cursor"));
	Cursor->SetScale(Vec2(30.f,30.f) * 2.f);
	///////////////////////커서/////////////////////////

	///////////////////////새 도전 글자/////////////////////////
	CSpriteUI* NewChallenge = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -150.f));
	NewChallenge->SetObjType(GROUP_TYPE::IMAGE);

	//NewChallenge->SetBackGround(true);
	//NewChallenge->SetBackGroundColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	NewChallenge->SetName(L"new_challenge");
	NewChallenge->AddImage(pD2DMgr->GetStoredBitmap(L"new_challenge"));
	NewChallenge->SetScale(Vec2(100.f, 48.f) * 2);
	
	///////////////////////새 도전 글자/////////////////////////

	///////////////////////계속하기 글자/////////////////////////
	CSpriteUI* RunContinue = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -90.f));
	RunContinue->SetObjType(GROUP_TYPE::IMAGE);

	//NewChallenge->SetBackGround(true);
	//NewChallenge->SetBackGroundColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	RunContinue->SetName(L"run_continue");
	RunContinue->AddImage(pD2DMgr->GetStoredBitmap(L"run_continue"));
	RunContinue->SetScale(Vec2(100.f, 48.f) * 2);
	
	///////////////////////계속하기 글자/////////////////////////



	///////////////////////설정 글자/////////////////////////
	CSpriteUI* Setting = GameMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -30.f));
	Setting->SetObjType(GROUP_TYPE::IMAGE);

	//Setting->SetBackGround(true);
	//Setting->SetBackGroundColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	Setting->SetName(L"setting");
	Setting->AddImage(pD2DMgr->GetStoredBitmap(L"setting"));
	Setting->SetScale(Vec2(100.f, 48.f) * 2);
	
	///////////////////////설정 글자/////////////////////////
}

void CScene_Main::CreateSelectCharacter(Vec2 _vResolution)
{
	////////////////////////////////////////////////////////////////
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"character_menu_panel",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(480.f, 270.f));
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CharacterCutting(i, j);
		}
	}
	////////////////////////////////////////////////////////////////
	CharacterNameCutting();
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	CharacterSelectArrowCutting();
	////////////////////////////////////////////////////////////////


	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* CharacterMenuBackGround = new CSpriteUI;
	CharacterMenuBackGround->SetObjType(GROUP_TYPE::IMAGE);
	CharacterMenuBackGround->SetName(L"character_background");
	CharacterMenuBackGround->AddImage(pD2DMgr->GetStoredBitmap(L"emptyscreen"));
	CharacterMenuBackGround->SetPos(Vec2(0.f, 1080.f));
	CharacterMenuBackGround->SetScale(_vResolution);
	AddObject(CharacterMenuBackGround, GROUP_TYPE::IMAGE);
	///////////////////////뒷 배경/////////////////////////

	///////////////////////캐릭터 선택 패널/////////////////////////
	CSpriteUI* CharacterPanel = CharacterMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f,0.f));
	CharacterPanel->SetObjType(GROUP_TYPE::IMAGE);
	CharacterPanel->SetName(L"character_panel");
	CharacterPanel->AddImage(pD2DMgr->GetStoredBitmap(L"character_menu_panel"));
	CharacterPanel->SetScale(_vResolution);
	
	///////////////////////캐릭터 선택 패널/////////////////////////

	/////////////////////////왼쪽 화살표////////////////////////////
	CSpriteUI* LeftArrow = CharacterMenuBackGround->AddChild<CSpriteUI>(Vec2(-150.f,20.f));
	LeftArrow->SetObjType(GROUP_TYPE::IMAGE);
	LeftArrow->SetName(L"character_select_left_arrow");
	LeftArrow->AddImage(pD2DMgr->GetStoredBitmap(L"character_select_left_arrow"));
	LeftArrow->SetScale(Vec2(25.f,20.f) * 2.f);
	
	/////////////////////////왼쪽 화살표////////////////////////////

	/////////////////////////오른쪽 화살표////////////////////////////
	CSpriteUI* RightArrow = CharacterMenuBackGround->AddChild<CSpriteUI>(Vec2(150.f, 20.f));
	RightArrow->SetObjType(GROUP_TYPE::IMAGE);
	RightArrow->SetName(L"character_select_right_arrow");
	RightArrow->AddImage(pD2DMgr->GetStoredBitmap(L"character_select_right_arrow"));
	RightArrow->SetScale(Vec2(25.f, 20.f) * 2.f);
	
	/////////////////////////오른쪽 화살표////////////////////////////


	///////////////////////캐릭터 선택 패널/////////////////////////
	m_CurShowingCharacter = CharacterMenuBackGround->AddChild<CSpriteUI>(Vec2(0.f, -80.f));
	m_CurShowingCharacter->SetObjType(GROUP_TYPE::IMAGE);
	m_CurShowingCharacter->SetName(L"character_panel");
	m_CurShowingCharacter->AddImage(pD2DMgr->GetStoredBitmap(L"isaac"));
	m_CurShowingCharacter->SetScale(Vec2(48.f, 48.f) * 2.f);
	
	///////////////////////캐릭터 선택 패널/////////////////////////

	///////////////////////캐릭터 선택 패널 캐릭터 이름/////////////////////////
	CSpriteUI* m_CurShowingCharacterName = m_CurShowingCharacter->AddChild<CSpriteUI>(Vec2(0.f, 96.f + 10.f));
	m_CurShowingCharacterName->SetObjType(GROUP_TYPE::IMAGE);
	m_CurShowingCharacterName->SetName(L"character_name");
	m_CurShowingCharacterName->AddImage(pD2DMgr->GetStoredBitmap(L"isaac_name"));
	m_CurShowingCharacterName->SetScale(Vec2(70.f, 30.f) * 2.f);
	
	///////////////////////캐릭터 선택 패널 캐릭터 이름/////////////////////////

}

void CScene_Main::CreateLoadImage(Vec2 _Resolution)
{
	int loadImageNum = rand() % 57;

	if (loadImageNum == 0) loadImageNum++;

	wstring loadImageNumString = std::to_wstring(loadImageNum); // int를 wstring으로 변환

	if (loadImageNum < 10)
		loadImageNumString = L"0" + loadImageNumString;

	wstring loadImageString = L"loadimages-0" + loadImageNumString;
	wstring loadImageString2 = loadImageString + L"_2";

	// 1. 리소스 매니저에서 비트맵 로드 (예시 구현 방식)
	ID2D1Bitmap* pBitmap1 = Direct2DMgr::GetInstance()->GetStoredBitmap(loadImageString);
	ID2D1Bitmap* pBitmap2 = Direct2DMgr::GetInstance()->GetStoredBitmap(loadImageString2);

	if (!pBitmap1 || !pBitmap2) {
		// 에러 처리
		return;
	}

	// 전체 합성 비트맵의 크기 계산: 
	// 좌측 여백 offset에서 시작해 gridCount * tileDrawSize 만큼 확장.
	const UINT compositeWidth = (UINT)(pBitmap1->GetSize().width * 2.f);
	const UINT compositeHeight = (UINT)(pBitmap1->GetSize().height * 2.f); // 정사각형

	// 3. 오프스크린 렌더 타겟(비트맵 렌더 타겟) 생성
	ID2D1Bitmap* pCompositeBitmap = nullptr;
	ComPtr<ID2D1BitmapRenderTarget> pBitmapRT = nullptr;
	HRESULT hr = Direct2DMgr::GetInstance()->GetRenderTarget()->CreateCompatibleRenderTarget(
		D2D1::SizeF((FLOAT)compositeWidth, (FLOAT)compositeHeight),
		&pBitmapRT
	);

	pBitmapRT->BeginDraw();
	// 배경색 (예: 흰색)으로 클리어
	pBitmapRT->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));


	// 4. 첫 번째 이미지 그리기 (좌측)
	D2D1_RECT_F destRect1 = D2D1::RectF(
		0.f,    // left
		0.f,    // top
		pBitmap1->GetSize().width,  // right (원본 이미지 너비)
		pBitmap1->GetSize().height   // bottom (원본 이미지 높이)
	);
	pBitmapRT->DrawBitmap(
		pBitmap1,
		destRect1,
		1.0f,   // opacity
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);

	// 5. 두 번째 이미지 그리기 (우측)
	D2D1_RECT_F destRect2 = D2D1::RectF(
		pBitmap1->GetSize().width,  // 좌측 이미지 옆에 배치
		0.f,
		pBitmap1->GetSize().width * 2.f,  // 112 * 2
		pBitmap1->GetSize().height
	);
	pBitmapRT->DrawBitmap(
		pBitmap2,
		destRect2,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);

	// 6. 렌더링 종료
	hr = pBitmapRT->EndDraw();

	pBitmapRT->GetBitmap(&pCompositeBitmap);

	
	veil = new CSpriteUI;
	veil->SetName(L"Veil");
	veil->SetObjType(GROUP_TYPE::IMAGE);
	veil->SetBackGround(true);
	veil->SetBackGroundColor(D2D1::ColorF(0.f, 0.f, 0.f, 1.f), D2D1::ColorF(0.f, 0.f, 0.f, 1.f)
		, D2D1::ColorF(0.f, 0.f, 0.f, 1.f), D2D1::ColorF(0.f, 0.f, 0.f, 1.f));
	veil->SetScale(_Resolution);
	veil->SetPos(Vec2(0.f,1080.f));
	AddObject(veil, GROUP_TYPE::IMAGE);

	CSpriteUI* loadImage = veil->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
	loadImage->SetObjType(GROUP_TYPE::IMAGE);
	loadImage->SetName(L"loadImage");
	loadImage->CreateAnimator();
	loadImage->GetAnimator()->CreateAnimation(L"loadImage", pCompositeBitmap
		, Vec2(0.f, 0.f), Vec2(pBitmap1->GetSize().width, pBitmap1->GetSize().height), Vec2(pBitmap1->GetSize().width, 0.f), 0.2f, 2, true, Vec2(0.f, 0.f));
	loadImage->GetAnimator()->Play(L"loadImage", true, 1);
	loadImage->SetScale(Vec2(pBitmap1->GetSize().width, pBitmap1->GetSize().height) * 2.f);
	loadImage->SetRenderScale(loadImage->GetScale() * 2.f);
}

void CScene_Main::CreateVeil(Vec2 _Resolution)
{
	
}

void CScene_Main::CharacterCutting(int i, int j)
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"characters",
		D2D1::Point2F(176.f, 288.f), D2D1::Point2F(176.f + 48.f * 7, 288.f + 48.f * 4));

	wstring name;

	if (j == 0 && i == 0) name = character_names[0];			//아이작
	else if (j == 1 && i == 0) name = character_names[1];		//막달레나
	else if (j == 0 && i == 2) name = character_names[2];		//카인
	else if (j == 1 && i == 2) name = character_names[3];		//유다

	/*
	else if (j == 0 && i == 4) name = character_names[4];		//???
	else if (j == 1 && i == 4) name = character_names[5];		//이브
	else if (j == 2 && i == 0) name = character_names[6];		//삼손
	else if (j == 2 && i == 2) name = character_names[7];		//아자젤
	else if (j == 2 && i == 4) name = character_names[8];		//나사로
	else if (j == 3 && i == 0) name = character_names[9];		//에덴
	else if (j == 0 && i == 5) name = character_names[10];		//더 로스트
	else if (j == 3 && i == 3) name = character_names[11];		//릴리스
	else if (j == 3 && i == 4) name = character_names[12];		//키퍼
	else if (j == 1 && i == 6) name = character_names[13];		//베타니
	*/

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"characters"), name,
		D2D1::Point2F(48.f * i, 48.f * j), D2D1::Point2F(48.f * (i+1), 48.f * (j + 1)));
}

void CScene_Main::CharacterNameCutting()
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"isaac_name",
		D2D1::Point2F(0.f, 292.f), D2D1::Point2F(0.f + 65.f, 292.f + 30.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"cain_name",
		D2D1::Point2F(0.f, 292.f + 30.f * 1.f), D2D1::Point2F(0.f + 65.f, 292.f + 30.f * 2.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"magdalene_name",
		D2D1::Point2F(0.f, 292.f + 30.f * 2.f), D2D1::Point2F(0.f + 65.f + 5.f, 292.f + 30.f * 3.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"judas_name",
		D2D1::Point2F(0.f, 292.f + 30.f * 3.f), D2D1::Point2F(0.f + 65.f , 292.f + 30.f * 4.f));
}

void CScene_Main::CharacterSelectArrowCutting()
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"character_select_left_arrow",
		D2D1::Point2F(0.f, 270.f), D2D1::Point2F(0.f + 25.f, 270.f + 20.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"charactermenu"), L"character_select_right_arrow",
		D2D1::Point2F(30.f, 270.f), D2D1::Point2F(30.f + 25.f, 270.f + 20.f));
}

void CScene_Main::MoveCursor(int direction)
{
	if (m_iCurPage != 1) return;

	m_iCursorPos = (m_iCursorPos + direction + 3) % 3;
	Cursor->SetOffset(CursorPos[m_iCursorPos]);
	CSoundMgr::GetInstance()->Play(L"menu_scroll");
}

void CScene_Main::ChangeCharacter(int direction)
{
	if (m_CurShowingCharacter == nullptr || m_iCurPage != 2) return;

	m_iCurCharacterIndex = (m_iCurCharacterIndex + direction + 4) % 4;

	UpdateCharacterImage();
	UpdateCharacterNameTag();

	CSoundMgr::GetInstance()->Play(direction > 0 ? L"character select right" : L"character select left");
}

void CScene_Main::UpdateCharacterImage()
{
	auto& images = m_CurShowingCharacter->GetImages();
	images[0]->SetBitmap(pD2DMgr->GetStoredBitmap(character_names[m_iCurCharacterIndex]));
}

void CScene_Main::UpdateCharacterNameTag()
{
	const auto& childUIs = m_CurShowingCharacter->GetChildsUI();
	auto& images = childUIs[0]->GetImages();
	images[0]->SetBitmap(pD2DMgr->GetStoredBitmap(character_names_tag[m_iCurCharacterIndex]));
}
