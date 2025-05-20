#include "global.h"
#include "CPlayer.h"
#include "CPickUpItem.h"

#include "CCollider.h"
#include "CAnimation.h"
#include "CAnimator.h"

#include "CSoundMgr.h"
#include "Direct2DMgr.h"
#include "CItemMgr.h"
#include "CPlayerMgr.h"


CPickUpItem::CPickUpItem(PICKUP_ITEM_TYPE _eType)
{
	m_ePickupType = _eType;
	CreateCollider();

	if (_eType == PICKUP_ITEM_TYPE::HEART)
	{
		SetScale(Vec2(32.f, 32.f) * 2.f);

		AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"pickup_drop_heart"));

		GetCollider()->SetScale(Vec2(16.f, 13.f)* 2.f);
		GetCollider()->SetOffsetPos(Vec2(-2.f, 0.f));

		CSoundMgr::GetInstance()->Play(L"heart drop", 0.5f);
	}
	else if (_eType == PICKUP_ITEM_TYPE::COIN)
	{
		SetScale(Vec2(32.f, 16.f) * 2.f);
		SetRenderScale(GetScale() * 2.f);

		CreateAnimator();
		GetAnimator()->CreateAnimation(L"pickup_drop_coin_animation", Direct2DMgr::GetInstance()->GetStoredBitmap(L"pickup_drop_coin_animation")
			, Vec2(0.f, 0.f), Vec2(32.f, 16.f), Vec2(32.f, 0.f), 0.05f, 6, true, Vec2(0.f, 0.f));
		GetAnimator()->Play(L"pickup_drop_coin_animation", true, 1);

		GetCollider()->SetScale(Vec2(18.f, 12.f) * 2.f);
		GetCollider()->SetOffsetPos(Vec2(0.f, 2.f));

		CSoundMgr::GetInstance()->Play(L"coin drop", 0.5f);
	}
	else if (_eType == PICKUP_ITEM_TYPE::KEY)
	{
		SetScale(Vec2(15.f, 21.f) * 2.f);
		AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"pickup_drop_key"));

		GetCollider()->SetScale(Vec2(15.f, 21.f) * 2.f);
		GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

		CSoundMgr::GetInstance()->Play(L"key drop", 0.5f);
	}
	else if (_eType == PICKUP_ITEM_TYPE::BOMB)
	{
		SetScale(Vec2(32.f, 32.f) * 2.f);
		AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"pickup_drop_bomb"));

		GetCollider()->SetScale(Vec2(16.f, 13.f) * 2.f);
		GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

		CSoundMgr::GetInstance()->Play(L"bomb drop", 0.5f);
	}

}

CPickUpItem::~CPickUpItem()
{

}

void CPickUpItem::start()
{

}

void CPickUpItem::update()
{
	CObject::update();
}

void CPickUpItem::finalupdate()
{
	CObject::finalupdate();
}

void CPickUpItem::render(ID2D1HwndRenderTarget* _pRender)
{
	CObject::render(_pRender);
}

void CPickUpItem::OnCollisionEnter(CCollider* _pOther)
{
	if (m_ePickupType == PICKUP_ITEM_TYPE::COIN)
	{
		CSoundMgr::GetInstance()->Play(L"coin pick", 0.5f);
		CItemMgr::GetInstance()->GetPickUpItem().m_iCoin++;
	}
	else if (m_ePickupType == PICKUP_ITEM_TYPE::BOMB)
	{
		CSoundMgr::GetInstance()->Play(L"bomb pick", 0.5f);
		CItemMgr::GetInstance()->GetPickUpItem().m_iBomb++;
	}
	else if (m_ePickupType == PICKUP_ITEM_TYPE::HEART)
	{
		CPlayerMgr::GetInstance()->PlayerHeal(1);
		CSoundMgr::GetInstance()->Play(L"heart drop", 0.5f);
	}
	else if (m_ePickupType == PICKUP_ITEM_TYPE::KEY)
	{
		CSoundMgr::GetInstance()->Play(L"key pick", 0.5f);
		CItemMgr::GetInstance()->GetPickUpItem().m_iKey++;
	}

	

	DeleteObject(this);
}

void CPickUpItem::OnCollision(CCollider* _pOther)
{
	
}

void CPickUpItem::OnCollisionExit(CCollider* _pOther)
{

}
