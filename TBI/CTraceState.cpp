#include "global.h"
#include "CTraceState.h"

#include "CPlayer.h"
#include "CPlayerMgr.h"
#include "CMonster.h"
#include "CTimeMgr.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{

}

void CTraceState::update()
{
	//타겟팅 된 Player를 쫒아간다. 
	CPlayer* pPlayer = CPlayerMgr::GetInstance()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonsterPos;
	vMonDir.Normalize();

	vMonsterPos += vMonDir * pMonster->Getinfo().m_fSpeed * fDT;

	pMonster->SetPos(vMonsterPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}
