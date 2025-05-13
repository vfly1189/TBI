#include "global.h"
#include "CHorfIdleState.h"

#include "CPlayer.h"
#include "CPlayerMgr.h"
#include "CMonster.h"
#include "MapMgr.h"

#include "CAnimation.h"
#include "CAnimator.h"

CHorfIdleState::CHorfIdleState()
	: CState(MON_STATE::IDLE)
{
}

CHorfIdleState::~CHorfIdleState()
{
}

void CHorfIdleState::update()
{
	CPlayer* pPlayer = (CPlayer*)CPlayerMgr::GetInstance()->GetPlayer();
	// 가만히 있는다. 

	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();

	if (GetMonster()->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}


	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	if ((vMonsterGridPos.x == vCurGridPos.x && vMonsterGridPos.y == vCurGridPos.y)
		&& fLen < pMonster->Getinfo().m_fRecogRange)
	{
		pMonster->GetAnimator()->PauseAllAnimations();
		pMonster->GetAnimator()->Play(L"horf_attack_anim", true, 1);

		ChangeAIState(GetAI(), MON_STATE::ATTACK);
	}

	// 플레이어의 위치를 받아와서 몬스터의 범위 안에 들어오면
	// 추적 상대료 전환. 
}

void CHorfIdleState::Enter()
{
}

void CHorfIdleState::Exit()
{
}