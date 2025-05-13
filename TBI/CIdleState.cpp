#include "global.h"
#include "CSceneMgr.h"
#include "CPlayerMgr.h"

#include "CMonster.h"
#include "CPlayer.h"

#include "CIdleState.h"

#include "MapMgr.h"



CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::update()
{
	CPlayer* pPlayer = (CPlayer*)CPlayerMgr::GetInstance()->GetPlayer();
	// 가만히 있는다. 

	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();

	printf("현재좌표 : %f %f , 몬스터 좌표 : %f %f\n", vMonsterGridPos.x, vMonsterGridPos.y, vCurGridPos.x, vCurGridPos.y);

	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	if ((vMonsterGridPos.x == vCurGridPos.x && vMonsterGridPos.y == vCurGridPos.y)
		&& fLen < pMonster->Getinfo().m_fRecogRange)
	{
		printf("상태변경\n");
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

	// 플레이어의 위치를 받아와서 몬스터의 범위 안에 들어오면
	// 추적 상대료 전환. 
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}