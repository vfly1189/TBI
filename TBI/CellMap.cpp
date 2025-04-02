#include "global.h"
#include "CellMap.h"

#include "MapMgr.h"
#include "Direct2DMgr.h"
#include "CPlayerMgr.h"

#include "CPlayer.h"
#include "CCollectiblesItem.h"
#include "CWall.h"
#include "CDoor.h"
#include "CSpriteUI.h"

#include "CCollider.h"
#include "CRigidBody.h"

Vec2 doorPosVec[4] = {
	Vec2(0.f,-215.f),
	Vec2(365.f, 0.f),
	Vec2(0.f, 215.f),
	Vec2(-365.f, 0.f),
};
	
CellMap::CellMap(wstring _strMapBaseSprite, Vec2 _vRealPos, Vec2 _vGridPos, ROOM_INFO _eRoomType)
	: m_bIsClear(false)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	m_vecDoorExist.resize(4);

	cellMap = new CSpriteUI;
	cellMap->SetObjType(GROUP_TYPE::IMAGE);
	cellMap->SetScale(Vec2(442.f, 286.f) * 2.f);
	cellMap->SetPos(_vRealPos);
	m_vMapPos = _vRealPos;
	m_vGridMapPos = _vGridPos;
	m_eRoomType = _eRoomType;
	cellMap->AddImage(pD2DMgr->GetStoredBitmap(_strMapBaseSprite));

	vector<vector<UINT>>& gridMap = MapMgr::GetInstance()->GetGridMap();
	for (int i = 0; i < 4; i++)
	{
		int ny = dy4[i] + (int)m_vGridMapPos.y;
		int nx = dx4[i] + (int)m_vGridMapPos.x;

		m_vecDoorExist[i] = false;

		if (ny < 0 || nx < 0
			|| ny >= (int)MapMgr::GetInstance()->GetMapMaxHeight() || nx >= (int)MapMgr::GetInstance()->GetMapMaxWidth())
			continue;
		if (gridMap[ny][nx] == (UINT)ROOM_INFO::EMPTY) continue;


		m_vecDoorExist[i] = true;
		//65x51
		CDoor* door = new CDoor;
		m_vecObjects.push_back(door);
		door->SetObjType(GROUP_TYPE::DOOR);
		door->SetDoorDir(i);
		door->CreateCollider();
		door->SetPos(doorPosVec[i] + _vRealPos);
		door->GetCollider()->SetScale(Vec2(22.f, 10.f) * 2.f);

		if (i == 0)
		{
			door->GetCollider()->SetOffsetPos(Vec2(0.f, -40.f));
			door->GetCollider()->SetScale(Vec2(22.f, 10.f) * 2.f);
		}
		else if (i == 1)
		{
			door->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
			door->GetCollider()->SetScale(Vec2(10.f, 24.f) * 2.f);
		}
			
		else if (i == 2)
		{
			door->GetCollider()->SetOffsetPos(Vec2(0.f, 40.f));
			door->GetCollider()->SetScale(Vec2(22.f, 10.f) * 2.f);
		}
			
		else if (i == 3) 
		{
			door->GetCollider()->SetOffsetPos(Vec2(-30.f, 0.f));
			door->GetCollider()->SetScale(Vec2(10.f, 24.f) * 2.f);
		}
			

		if		(i == 0) door->SetRotation(0.f);
		else if (i == 1) door->SetRotation(90.f);
		else if (i == 2) door->SetRotation(180.f);
		else if (i == 3) door->SetRotation(270.f);
		
		if (gridMap[ny][nx] == (UINT)ROOM_INFO::BOSS || m_eRoomType == ROOM_INFO::BOSS)
		{
			door->AddImage(pD2DMgr->GetStoredBitmap(L"bossroom_door"));
			door->SetScale(Vec2(65.f, 49.f) * 2.f);
		}
		else if (gridMap[ny][nx] == (UINT)ROOM_INFO::TREASURE || m_eRoomType == ROOM_INFO::TREASURE)
		{
			door->AddImage(pD2DMgr->GetStoredBitmap(L"treasureroom_door"));
			door->SetScale(Vec2(65.f, 49.f) * 2.f);
		}
		else if (gridMap[ny][nx] == (UINT)ROOM_INFO::NORMAL || gridMap[ny][nx] == (UINT)ROOM_INFO::START)
		{
			door->AddImage(pD2DMgr->GetStoredBitmap(L"normal_door"));
			door->SetScale(Vec2(49.f, 33.f) * 2.f);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		//문이 없을 경우
		if (m_vecDoorExist[i] == false)
		{
			CWall* wall = new CWall;
			wall->SetObjType(GROUP_TYPE::WALL);
			m_vecObjects.push_back(wall);
		
			if (i == 0)
			{
				wall->SetPos(_vRealPos + Vec2(0.f, -230.f));
				wall->SetScale(Vec2(700.f, 20.f));
			}
			
			else if (i == 1)
			{	
				wall->SetPos(_vRealPos + Vec2(350.f, 0.f));
				wall->SetScale(Vec2(20.f, 400.f));
			}

			else if (i == 2)
			{
				wall->SetPos(_vRealPos + Vec2(0.f, 200.f));
				wall->SetScale(Vec2(700.f, 20.f));
			}

			else if (i == 3)
			{	
				wall->SetPos(_vRealPos + Vec2(-350.f, 0.f));
				wall->SetScale(Vec2(20.f, 400.f));
			}
		}

		//문이 있을 경우
		else
		{
			CWall* wall_1 = new CWall;
			wall_1->SetObjType(GROUP_TYPE::WALL);
			CWall* wall_2 = new CWall;
			wall_2->SetObjType(GROUP_TYPE::WALL);
			m_vecObjects.push_back(wall_1);
			m_vecObjects.push_back(wall_2);

			if (i == 0)
			{
				wall_1->SetPos(_vRealPos + Vec2(-185.f, -230.f));
				wall_1->SetScale(Vec2(320.f, 20.f));

				wall_2->SetPos(_vRealPos + Vec2(185.f, -230.f));
				wall_2->SetScale(Vec2(320.f, 20.f));
			}

			else if (i == 1)
			{
				wall_1->SetPos(_vRealPos + Vec2(350.f, 120.f));
				wall_1->SetScale(Vec2(20.f, 180.f));

				wall_2->SetPos(_vRealPos + Vec2(350.f, -120.f));
				wall_2->SetScale(Vec2(20.f, 180.f));
			}

			else if (i == 2)
			{
				wall_1->SetPos(_vRealPos + Vec2(-185.f, 200.f));
				wall_1->SetScale(Vec2(320.f, 20.f));

				wall_2->SetPos(_vRealPos + Vec2(185.f, 200.f));
				wall_2->SetScale(Vec2(320.f, 20.f));
			}

			else if (i == 3)
			{
				wall_1->SetPos(_vRealPos + Vec2(-350.f, 120.f));
				wall_1->SetScale(Vec2(20.f, 180.f));

				wall_2->SetPos(_vRealPos + Vec2(-350.f, -120.f));
				wall_2->SetScale(Vec2(20.f, 180.f));
			}
		}
	}

	if (m_eRoomType == ROOM_INFO::TREASURE)
	{
		int randNum = rand() % 35;

		wstring item_image_tag = L"collectibles_";
		wstring item_number;

		if (randNum < 10)
			item_number = L"00" + std::to_wstring(randNum) + L"_";
		else if (randNum < 100)
			item_number = L"0" + std::to_wstring(randNum) + L"_";
		else
			item_number = std::to_wstring(randNum) + L"_";

		item_image_tag += item_number + items_tags[randNum - 1];

		CCollectiblesItem* newItem = new CCollectiblesItem(m_vMapPos, randNum);
		newItem->SetObjType(GROUP_TYPE::ITEM);
		newItem->SetScale(Vec2(100.f, 100.f));

		m_vecObjects.push_back(newItem);
	}
	else if (m_eRoomType == ROOM_INFO::START)
	{
		CreateControlExplain();
	}
}

CellMap::~CellMap()
{

}

void CellMap::CreateControlExplain()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	CPlayer* player = CPlayerMgr::GetInstance()->GetPlayer();
	int characterIdx = player->GetCharactIdx();


	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls"), L"control",
		D2D1::Point2F(0.f, 97.f * characterIdx), D2D1::Point2F(325.f, 97.f * (characterIdx + 1)));

	//w
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_w",
		D2D1::Point2F(96.f, 16.f), D2D1::Point2F(96.f + 16.f, 16.f + 16.f));
	//a
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_a",
		D2D1::Point2F(0.f, 0.f), D2D1::Point2F(0.f + 16.f, 0.f + 16.f));
	//s
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_s",
		D2D1::Point2F(32.f, 16.f), D2D1::Point2F(32.f + 16.f, 16.f + 16.f));
	//d
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_d",
		D2D1::Point2F(48.f, 0.f), D2D1::Point2F(48.f + 16.f, 0.f + 16.f));
	//e
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_e",
		D2D1::Point2F(64.f, 0.f), D2D1::Point2F(64.f + 16.f, 0.f + 16.f));
	//space
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_space",
		D2D1::Point2F(0.f, 48.f), D2D1::Point2F(0.f + 32.f, 48.f + 16.f));


	CSpriteUI* controlExplain = cellMap->AddChild<CSpriteUI>(Vec2(0.f, -20.f));
	controlExplain->SetScale(Vec2(325.f, 97.f) * 2.f);
	controlExplain->AddImage(pD2DMgr->GetStoredBitmap(L"control"));

	CSpriteUI* controlButtonW = cellMap->AddChild<CSpriteUI>(Vec2(-280.f, 50.f));
	controlButtonW->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonW->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_w"));

	CSpriteUI* controlButtonA = cellMap->AddChild<CSpriteUI>(Vec2(-255.f, 50.f));
	controlButtonA->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonA->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_a"));

	CSpriteUI* controlButtonS = cellMap->AddChild<CSpriteUI>(Vec2(-230.f, 50.f));
	controlButtonS->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonS->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_s"));

	CSpriteUI* controlButtonD = cellMap->AddChild<CSpriteUI>(Vec2(-205.f, 50.f));
	controlButtonD->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonD->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_d"));




	//위 화살표
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_up_arrow",
		D2D1::Point2F(128.f, 64.f), D2D1::Point2F(128.f + 16.f, 64.f + 16.f));
	//아래 화살표
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_down_arrow",
		D2D1::Point2F(112.f, 64.f), D2D1::Point2F(112.f + 16.f, 64.f + 16.f));
	//오른쪽 화살표
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_right_arrow",
		D2D1::Point2F(80.f, 64.f), D2D1::Point2F(80.f + 16.f, 64.f + 16.f));
	//아래 화살표
	pD2DMgr->SplitBitmap(pD2DMgr->GetStoredBitmap(L"controls_buttons"), L"control_button_left_arrow",
		D2D1::Point2F(96.f, 64.f), D2D1::Point2F(96.f + 16.f, 64.f + 16.f));


	CSpriteUI* controlButtonUpArrow = cellMap->AddChild<CSpriteUI>(Vec2(-115.f, 50.f));
	controlButtonUpArrow->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonUpArrow->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_up_arrow"));

	CSpriteUI* controlButtonLeftArrow = cellMap->AddChild<CSpriteUI>(Vec2(-90.f, 50.f));
	controlButtonLeftArrow->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonLeftArrow->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_left_arrow"));

	CSpriteUI* controlButtonDownArrow = cellMap->AddChild<CSpriteUI>(Vec2(-65.f, 50.f));
	controlButtonDownArrow->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonDownArrow->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_down_arrow"));

	CSpriteUI* controlButtonRightArrow = cellMap->AddChild<CSpriteUI>(Vec2(-40.f, 50.f));
	controlButtonRightArrow->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonRightArrow->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_right_arrow"));


	CSpriteUI* controlButtonE = cellMap->AddChild<CSpriteUI>(Vec2(90.f, 50.f));
	controlButtonE->SetScale(Vec2(16.f, 16.f) * 2.f);
	controlButtonE->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_e"));

	CSpriteUI* controlButtonSpace = cellMap->AddChild<CSpriteUI>(Vec2(260.f, 50.f));
	controlButtonSpace->SetScale(Vec2(32.f, 16.f) * 2.f);
	controlButtonSpace->AddImage(pD2DMgr->GetStoredBitmap(L"control_button_space"));
}

void CellMap::update()
{
	cellMap->update();
	for (size_t i = 0; i < m_vecDoors.size(); i++)
		m_vecDoors[i]->update();
}

void CellMap::finalupdate()
{
	cellMap->finalupdate();
	for (size_t i = 0; i < m_vecDoors.size(); i++)
		m_vecDoors[i]->finalupdate();
}

void CellMap::render(ID2D1HwndRenderTarget* _pRender)
{
	cellMap->render(_pRender);
	for (size_t i = 0; i < m_vecDoors.size(); i++)
		m_vecDoors[i]->render(_pRender);
}
