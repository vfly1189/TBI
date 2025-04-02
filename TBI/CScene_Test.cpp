#include "global.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CWall.h"

#include "CUI.h"
#include "CSpriteUI.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CImage.h"

#include "CCore.h"
#include "CSoundMgr.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "MapMgr.h"
#include "Direct2DMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CPlayerMgr.h"

#include "CScene_Test.h"

CScene_Test::CScene_Test()
{

}

CScene_Test::~CScene_Test()
{

}

void CScene_Test::Enter()
{
	printf("SCENE_TEST\n");
	pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	MapMgr::GetInstance()->MapGenerate();
	MapMgr::GetInstance()->ShowMap();



	
	//CreateMain(vResolution);
/*
	MapMgr::GetInstance()->MapGenerate();
	MapMgr::GetInstance()->ShowMap();
	
	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	player->SetPos(Vec2(0.f, 0.f));
	player->SetName(L"Player");
	player->SetObjType(GROUP_TYPE::PLAYER);
	player->SetScale(Vec2(32.f,32.f));
	player->SetRenderScale(player->GetScale() * 2.f);
	AddObject(player, GROUP_TYPE::PLAYER);
	//player->AddImage(pD2DMgr->GetStoredBitmap(L"01_basement"));
	//player->GetAnimator()->Play(L"left_walking", true);
	
	
	CreateDoor(vResolution);

	//CreateWall(vResolution);
	/*
	///////////////////////뒷 배경/////////////////////////
	CSpriteUI* MainPanel = new CSpriteUI;
	MainPanel->SetObjType(GROUP_TYPE::IMAGE);
	MainPanel->SetName(L"player_animation_sprite");
	MainPanel->AddImage(pD2DMgr->GetStoredBitmap(L"test"));
	MainPanel->SetPos(Vec2(0.f, 0.f));
	MainPanel->SetScale(Vec2(32.f,32.f));
	AddObject(MainPanel, GROUP_TYPE::IMAGE);
	///////////////////////뒷 배경/////////////////////////
	*/

	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL);
	wstring mainTitleBGMKey = L"genesis_retake_light_loop";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.2f);
	}
	start();
}

void CScene_Test::Exit()
{
	DeleteAll();
}

void CScene_Test::update()
{
	CScene::update();
	MapMgr::GetInstance()->update();
	//CPlayerMgr::GetInstance()->GetPlayer()->update();
	if (KEY_TAP(KEY::ENTER))
	{
		MapMgr::GetInstance()->MapGenerate();
		MapMgr::GetInstance()->ShowMap();
	}
}

void CScene_Test::finalupdate()
{

	CScene::finalupdate();
	MapMgr::GetInstance()->finalupdate();
	//CPlayerMgr::GetInstance()->GetPlayer()->finalupdate();
}

void CScene_Test::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
	MapMgr::GetInstance()->render(_pRender);
	//CPlayerMgr::GetInstance()->GetPlayer()->render(_pRender);
}


void CScene_Test::CreateMain(Vec2 _vResolution)
{
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"01_basement"), L"basement1",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(234.f, 156.f));

	ID2D1Bitmap* originalBitmap = pD2DMgr->GetStoredBitmap(L"basement1");
	
	ID2D1Bitmap* xFlip = FlipBitamp(originalBitmap, true, false);
	ID2D1Bitmap* yFlip = FlipBitamp(originalBitmap, false, true);
	ID2D1Bitmap * xyFlip = FlipBitamp(originalBitmap, true, true);

	vector<ID2D1Bitmap*> bitmaps;
	bitmaps.push_back(originalBitmap);
	bitmaps.push_back(xFlip);
	bitmaps.push_back(yFlip);
	bitmaps.push_back(xyFlip);

	ID2D1Bitmap* combinedBitmap = CombineBitmaps2X2(bitmaps);

	CSpriteUI* test = new CSpriteUI;
	test->SetObjType(GROUP_TYPE::IMAGE);
	test->SetScale(Vec2(442.f, 286.f) * 2.f);
	test->SetPos(Vec2(0.f, 0.f));
	test->AddImage(combinedBitmap);
	AddObject(test, GROUP_TYPE::IMAGE);
}

void CScene_Test::CreateDoor(Vec2 _vResolution)
{
	//북쪽문 : 0,-215
	//동쪽문 : 350, -15

	//49 , 33
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"door_01_normaldoor"), L"normal_door",
		D2D1::Point2F(9.f, 10.f), D2D1::Point2F(9.f + 49.f , 10.f + 33.f));

	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"door_01_normaldoor"), L"normal_door_open",
		D2D1::Point2F(65.f, 0.f), D2D1::Point2F(130.f, 51.f));


	//65x51
	CSpriteUI* doorUp = new CSpriteUI;
	doorUp->CreateCollider();
	doorUp->GetCollider()->SetScale(Vec2(22.f, 24.f) * 2.f);
	doorUp->GetCollider()->SetOffsetPos(Vec2(0.f, -20.f));
	doorUp->SetObjType(GROUP_TYPE::IMAGE);
	doorUp->SetPos(Vec2(0.f, -214.f));
	doorUp->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door"));
	doorUp->SetScale(Vec2(49.f, 33.f) * 2.f);
	AddObject(doorUp, GROUP_TYPE::IMAGE);

	//65x51
	CSpriteUI* doorRight = new CSpriteUI;
	doorRight->CreateCollider();
	doorRight->GetCollider()->SetScale(Vec2(22.f, 24.f) * 2.f);
	doorRight->GetCollider()->SetOffsetPos(Vec2(15.f, 0.f));

	doorRight->SetObjType(GROUP_TYPE::IMAGE);
	doorRight->SetPos(Vec2(358.f, 0.f));
	doorRight->SetScale(Vec2(49.f,33.f) * 2.f);
	doorRight->SetRotation(90.f);
	doorRight->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door"));
	AddObject(doorRight, GROUP_TYPE::IMAGE);
	
	//65x51
	CSpriteUI* doorDown = new CSpriteUI;
	doorDown->CreateCollider();
	doorDown->GetCollider()->SetScale(Vec2(22.f, 24.f) * 2.f);
	doorDown->GetCollider()->SetOffsetPos(Vec2(0.f, 20.f));

	doorDown->SetObjType(GROUP_TYPE::IMAGE);
	doorDown->SetPos(Vec2(0.f, 214.f));
	doorDown->SetScale(Vec2(49.f, 33.f) * 2.f);
	doorDown->SetRotation(180.f);
	doorDown->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door"));
	AddObject(doorDown, GROUP_TYPE::IMAGE);

	//65x51
	CSpriteUI* doorLeft = new CSpriteUI;
	doorLeft->CreateCollider();
	doorLeft->GetCollider()->SetScale(Vec2(22.f, 24.f) * 2.f);
	doorLeft->GetCollider()->SetOffsetPos(Vec2(-15.f, 0.f));

	doorLeft->SetObjType(GROUP_TYPE::IMAGE);
	doorLeft->SetPos(Vec2(-358.f, 0.f));
	doorLeft->SetScale(Vec2(49.f, 33.f) * 2.f);
	doorLeft->SetRotation(270.f);
	doorLeft->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door"));
	AddObject(doorLeft, GROUP_TYPE::IMAGE);

	/*
	//65x51
	CSpriteUI* door_open = door->AddChild<CSpriteUI>(Vec2(2.f,0.f));
	door_open->SetObjType(GROUP_TYPE::IMAGE);
	door_open->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door_open"));
	door_open->SetScale(Vec2(65.f, 51.f) * 2.f);*/
}

void CScene_Test::CreateWall(Vec2 _vResolution)
{
	CWall* upWall_01 = new CWall;
	upWall_01->SetScale(Vec2(288.f, 57.f));
	upWall_01->SetPos(Vec2(-190.f, -240.f));
	AddObject(upWall_01, GROUP_TYPE::WALL);
}
