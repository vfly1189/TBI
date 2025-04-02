#include "global.h"
#include "CellMap.h"

#include "MapMgr.h"
#include "Direct2DMgr.h"

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
}

CellMap::~CellMap()
{

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
