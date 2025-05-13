#include "global.h"
#include "CHorfDeadState.h"

#include "CMonster.h";
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CSoundMgr.h"

#include "CPickUpItem.h"

CHorfDeadState::CHorfDeadState()
	:CState(MON_STATE::DEAD)
{

}

CHorfDeadState::~CHorfDeadState()
{

}

void CHorfDeadState::update()
{
	Vec2 vMonsterPos = GetMonster()->GetPos();


	m_fAccTime += fDT;

	if (m_fAccTime >= 0.36f)
	{
		CreatePickupItem(vMonsterPos);
		DeleteObject(GetMonster());

	
		return;
	}
}

void CHorfDeadState::Enter()
{
	GetMonster()->GetAnimator()->PauseAllAnimations();
	GetMonster()->GetAnimator()->Play(L"mon_dead_anim", false, 1);
	CSoundMgr::GetInstance()->Play(L"death burst small 2");
}

void CHorfDeadState::Exit()
{
}

void CHorfDeadState::CreatePickupItem(Vec2 _vPos)
{
	int possibility = rand() % 100;
	CPickUpItem* testItem = nullptr;

	if (possibility <= 30) testItem = new CPickUpItem(PICKUP_ITEM_TYPE::HEART);
	else if (possibility > 30 && possibility <= 50) testItem = new CPickUpItem(PICKUP_ITEM_TYPE::COIN);
	else if (possibility > 50 && possibility <= 70) testItem = new CPickUpItem(PICKUP_ITEM_TYPE::KEY);
	else if (possibility > 70 && possibility <= 90) testItem = new CPickUpItem(PICKUP_ITEM_TYPE::BOMB);

	if (possibility <= 90)
	{
		testItem->SetPos(_vPos);
		testItem->SetObjType(GROUP_TYPE::PICKUP_ITEM);
		CreateObject(testItem, GROUP_TYPE::PICKUP_ITEM);
	}
}
