#include "global.h"
#include "CBabyPlumIdleState.h"

#include "CSceneMgr.h"
#include "CPlayerMgr.h"

#include "CMonster.h"
#include "CPlayer.h"


#include "MapMgr.h"
#include "CSoundMgr.h"

#include "CAnimation.h"
#include "CAnimator.h"

CBabyPlumIdleState::CBabyPlumIdleState()
	: CState(MON_STATE::IDLE)
{

}


CBabyPlumIdleState::~CBabyPlumIdleState()
{

}

void CBabyPlumIdleState::update()
{
	if (GetMonster()->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}

	CPlayer* pPlayer = (CPlayer*)CPlayerMgr::GetInstance()->GetPlayer();
	// 가만히 있는다. 

	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();


	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	if ((vMonsterGridPos.x == vCurGridPos.x && vMonsterGridPos.y == vCurGridPos.y)
		&& fLen < pMonster->Getinfo().m_fRecogRange)
	{
		printf("변환\n");
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
	// 플레이어의 위치를 받아와서 몬스터의 범위 안에 들어오면
	// 추적 상대료 전환. 
}

void CBabyPlumIdleState::Enter()
{
}

void CBabyPlumIdleState::Exit()
{
}

