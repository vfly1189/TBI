#include "global.h"

#include "CObject.h"
#include "CSpriteUI.h"
#include "CCollectiblesItem.h"

#include "CCollider.h"

#include "Direct2DMgr.h"

CCollectiblesItem::CCollectiblesItem(Vec2 _vPos, int _iNum)
{
	SetPos(_vPos);
	m_stItemInfo = items[_iNum - 1];

	CreateCollider();
	GetCollider()->SetScale(Vec2(25.f, 25.f) * 2.f);

}

CCollectiblesItem::~CCollectiblesItem()
{

}


void CCollectiblesItem::start()
{
	CSpriteUI* pedestal = new CSpriteUI;
	m_vecObjects.push_back(pedestal);
	pedestal->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"item_pedestal"));
	pedestal->SetPos(GetPos());
	pedestal->SetScale(Vec2(32.f, 32.f) * 2.f);
	pedestal->SetObjType(GROUP_TYPE::IMAGE);

	CSpriteUI* item_shadow = pedestal->AddChild<CSpriteUI>(Vec2(0.f, -10.f));
	item_shadow->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"item_pedestal_shadow"));
	item_shadow->SetScale(Vec2(13.5f, 5.f) * 2.f);


	CSpriteUI* selectedItem = pedestal->AddChild<CSpriteUI>(Vec2(0.f, -45.f));
	
	wstring item_image_tag = L"collectibles_";
	if (m_stItemInfo.m_iNumber < 10)
		item_image_tag += L"00" + std::to_wstring(m_stItemInfo.m_iNumber);
	else if (m_stItemInfo.m_iNumber < 100)
		item_image_tag += L"0" + std::to_wstring(m_stItemInfo.m_iNumber);
	else
		item_image_tag += std::to_wstring(m_stItemInfo.m_iNumber);

	item_image_tag += L"_" + m_stItemInfo.m_sItemTag;

	selectedItem->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(item_image_tag));
	selectedItem->SetScale(Vec2(32.f, 32.f) * 2.f);
}

void CCollectiblesItem::update()
{
	CObject::update();
	for (size_t i = 0; i < m_vecObjects.size(); i++)
		m_vecObjects[i]->update();
}

void CCollectiblesItem::finalupdate()
{
	CObject::finalupdate();
	for (size_t i = 0; i < m_vecObjects.size(); i++)
		m_vecObjects[i]->finalupdate();
}

void CCollectiblesItem::render(ID2D1HwndRenderTarget* _pRender)
{
	CObject::render(_pRender);
	for (size_t i = 0; i < m_vecObjects.size(); i++)
	{
		m_vecObjects[i]->render(_pRender);
	}
}

void CCollectiblesItem::OnCollisionEnter(CCollider* _pOther)
{
}

void CCollectiblesItem::OnCollision(CCollider* _pOther)
{
	DeleteObject(this);
	
	/*
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {

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
	}*/
}

void CCollectiblesItem::OnCollisionExit(CCollider* _pOther)
{
}
