#include "global.h"

#include "Direct2DMgr.h"
#include "CCore.h"
#include "MapMgr.h"
#include "CCollisionMgr.h"
#include "CSoundMgr.h"
#include "CPlayerMgr.h"
#include "CKeyMgr.h"

#include "CScene_Fight.h"

#include "CDoor.h"
#include "CScene.h"
#include "CWall.h"
#include "CPlayer.h"
#include "CSpriteUI.h"
#include "CellMap.h"

CScene_Fight::CScene_Fight()
{

}

CScene_Fight::~CScene_Fight()
{

}


void CScene_Fight::Enter()
{
	printf("SCENE_FIGHT\n");
	pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	CCamera::GetInstance()->SetLookAt(Vec2(0.f, 0.f));

	MapMgr::GetInstance()->MapGenerate();
	MapMgr::GetInstance()->ShowMap();

	
	vector<CellMap*>& cellMaps = MapMgr::GetInstance()->GetCellMaps();
	for (size_t i = 0; i < cellMaps.size(); ++i)
	{
		AddObject((CObject*)cellMaps[i]->GetCellMap(), GROUP_TYPE::IMAGE);

		vector<CObject*>& cellMapObjects = cellMaps[i]->GetObjects();	

		for (size_t j = 0; j < cellMapObjects.size(); j++)
		{
			GROUP_TYPE groupType = cellMapObjects[j]->GetObjType();

			AddObject(cellMapObjects[j], groupType);
		}	
	}

	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	player->SetPos(Vec2(0.f, 0.f));
	player->SetName(L"PLAYER");
	player->SetObjType(GROUP_TYPE::PLAYER);
	player->SetScale(Vec2(32.f, 32.f));
	player->SetRenderScale(player->GetScale() * 2.f);
	AddObject(player, GROUP_TYPE::PLAYER);

	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::ITEM);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DOOR);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::WALL);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::TEAR, GROUP_TYPE::DOOR);

	wstring mainTitleBGMKey = L"genesis_retake_light_loop";
	//처음 시작했을 때, BGM틀기. 
	if (!CSoundMgr::GetInstance()->IsPlaySound(mainTitleBGMKey)) {
		CSoundMgr::GetInstance()->Play(mainTitleBGMKey, 0.2f);
	}
	start();
}

void CScene_Fight::Exit()
{
	DeleteAll();
}

void CScene_Fight::update()
{
	if (KEY_TAP(KEY::ENTER))
	{
		MapMgr::GetInstance()->MapGenerate();
		MapMgr::GetInstance()->ShowMap();
	}

	//printf("카메라 : %f %f\n", CCamera::GetInstance()->GetLookAt().x, CCamera::GetInstance()->GetLookAt().y);

	CScene::update();
	//MapMgr::GetInstance()->update();
}

void CScene_Fight::finalupdate()
{
	CScene::finalupdate();
	//MapMgr::GetInstance()->finalupdate();
}

void CScene_Fight::render(ID2D1HwndRenderTarget* _pRender)
{
	//MapMgr::GetInstance()->render(_pRender);
	CScene::render(_pRender);
}

