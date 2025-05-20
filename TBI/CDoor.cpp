#include "global.h"
#include "CDoor.h"

#include "CObject.h"
#include "CellMap.h"

#include "CCollider.h"

#include "CCamera.h"

#include "Direct2DMgr.h"
#include "CSoundMgr.h"

CDoor::CDoor()
{
	//CreateCollider();
}

CDoor::~CDoor()
{
}

void CDoor::start()
{

}

void CDoor::update()
{
	CObject::update();

	if (m_pOwner->GetMobCount() == 0 && m_bIsOpen == false)
	{
		m_bIsOpen = true;


		CreateCollider();

		if (m_iDoorDir == 0)
		{
			GetCollider()->SetOffsetPos(Vec2(0.f, -30.f));
			GetCollider()->SetScale(Vec2(22.f, 10.f) * 2.f);
		}
		else if (m_iDoorDir == 1)
		{
			GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
			GetCollider()->SetScale(Vec2(10.f, 24.f) * 2.f);
		}

		else if (m_iDoorDir == 2)
		{
			GetCollider()->SetOffsetPos(Vec2(0.f, 30.f));
			GetCollider()->SetScale(Vec2(22.f, 10.f) * 2.f);
		}

		else if (m_iDoorDir == 3)
		{
			GetCollider()->SetOffsetPos(Vec2(-30.f, 0.f));
			GetCollider()->SetScale(Vec2(10.f, 24.f) * 2.f);
		}

		DeleteImages();
		if (m_eRoomType == ROOM_INFO::NORMAL)
		{
			AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"normal_door_open"));
			AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"normal_door"));
		}
		else if (m_eRoomType == ROOM_INFO::TREASURE)
		{
			AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"treasureroom_door_open"));
			AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"treasureroom_door"));
		}
		else if (m_eRoomType == ROOM_INFO::BOSS)
		{
			AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"bossroom_door_open"));
			AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"bossroom_door"));
		}
	}
}

void CDoor::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

void CDoor::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetObjType() == GROUP_TYPE::PLAYER)
	{
		if (m_bIsOpen)
		{
			Vec2 vPos = CCamera::GetInstance()->GetLookAt();
			if (m_iDoorDir == 0)
			{
				vPos -= Vec2(0.f, 540.f);
			}
			else if (m_iDoorDir == 1)
			{
				vPos += Vec2(960.f, 0.f);
			}
			else if (m_iDoorDir == 2)
			{
				vPos += Vec2(0.f, 540.f);
			}
			else if (m_iDoorDir == 3)
			{
				vPos -= Vec2(960.f, 0.f);
			}
			CCamera::GetInstance()->SetLookAt(vPos);
		}
	}
}

void CDoor::OnCollision(CCollider* _pOther)
{
	if (!m_bIsOpen)
	{
		CObject* pOtherObj = _pOther->GetObj();
		if (pOtherObj->GetName() == L"PLAYER") {

			//파고 들었을 경우에 중심 위치에서. 
			Vec2 vObjPos = _pOther->GetFinalPos();
			Vec2 vObjScale = _pOther->GetScale();

			Vec2 vPos = GetCollider()->GetFinalPos();
			Vec2 vScale = GetCollider()->GetScale();

			if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
				&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
				&& (vObjPos.y < vPos.y))
			{
				//Case 1 캐릭터가 위에 있을 때. 
				// X축 값으로 사이에 있고, 플레이어 Y값이 Ground보다 낮을 경우. 
				float fLen = abs(vObjPos.y - vPos.y);
				float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

				vObjPos = pOtherObj->GetPos();
				vObjPos.y -= (fInterpolValue);
				pOtherObj->SetPos(vObjPos);
			}
			else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
				&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
				&& (vPos.x < vObjPos.x)) {
				//Case 2 캐릭터가 오른쪽에 있을 때.
				//Y축 값으로 사이에 있고, 플레이어 X값이 Ground보다 높을 경우

				float fLen = abs(vObjPos.x - vPos.x);
				float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

				vObjPos = pOtherObj->GetPos();
				vObjPos.x += (fInterpolValue);
				pOtherObj->SetPos(vObjPos);

			}
			else if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
				&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
				&& (vObjPos.y > vPos.y)) {
				//Case 3 캐릭터가 아래에 있을 때.
				// X축 값으로 사이에 있고, 플레이어 Y값이 Ground보다 높을 경우. 
				float fLen = abs(vObjPos.y - vPos.y);
				float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

				vObjPos = pOtherObj->GetPos();
				vObjPos.y += (fInterpolValue);
				pOtherObj->SetPos(vObjPos);
			}
			else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
				&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
				&& (vPos.x > vObjPos.x)) {
				//Case 4 캐릭터가 왼쪽에 있을 때. 
				//Y축 값으로 사이에 있고, 플레이어 X값이 Ground보다 낮을 경우
				float fLen = abs(vObjPos.x - vPos.x);
				float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

				vObjPos = pOtherObj->GetPos();
				vObjPos.x -= (fInterpolValue);
				pOtherObj->SetPos(vObjPos);
			}
		}
	}
}

void CDoor::OnCollisionExit(CCollider* _pOther)
{

}
