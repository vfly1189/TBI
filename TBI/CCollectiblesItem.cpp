#include "global.h"

#include "CObject.h"
#include "CSpriteUI.h"

#include "CCollectiblesItem.h"

#include "CCollider.h"

#include "Direct2DMgr.h"
#include "CItemMgr.h"

CCollectiblesItem::CCollectiblesItem(Vec2 _vPos, int _iNum)
{
	SetPos(_vPos);
	m_stItemInfo = items[_iNum - 1];
	m_eItemType = m_stItemInfo.m_eItemType;

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


	selectedItem = pedestal->AddChild<CSpriteUI>(Vec2(0.f, -45.f));
	
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
	if (m_eItemType == COLLECTIBLES_ITEM_TYPE::PASSIVE)
		DeleteObject(this);
}

void CCollectiblesItem::OnCollision(CCollider* _pOther)
{
	
	
}

void CCollectiblesItem::OnCollisionExit(CCollider* _pOther)
{
}
