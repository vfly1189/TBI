#include "global.h"
#include "CFlyTraceState.h"

#include "CPlayer.h"
#include "CSoundMgr.h"
#include "CPlayerMgr.h"
#include "CMonster.h"
#include "MapMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"

#include "CScene_Fight.h"

CFlyTraceState::CFlyTraceState()
	: CState(MON_STATE::TRACE)
{
}

CFlyTraceState::~CFlyTraceState()
{

}

void CFlyTraceState::update()
{
	if (dynamic_cast<CScene_Fight*>(CSceneMgr::GetInstance()->GetCurScene())->IsGameOver()==true)
	{
		return;
	}


	//타겟팅 된 Player를 쫒아간다. 
	CPlayer* pPlayer = CPlayerMgr::GetInstance()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();

	if (pMonster->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
	}
	////////////////////방 벗어나면추적 종료/////////////
	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	if ((vMonsterGridPos.x != vCurGridPos.x || vMonsterGridPos.y != vCurGridPos.y)
		|| fLen > pMonster->Getinfo().m_fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::IDLE);
		return;
	}
	////////////////////방 벗어나면추적 종료/////////////

	Vec2 vMonDir = vPlayerPos - vMonsterPos;
	vMonDir.Normalize();

	vMonsterPos += vMonDir * pMonster->Getinfo().m_fSpeed * fDT;
	pMonster->SetPos(vMonsterPos);
}

void CFlyTraceState::Enter()
{
	
}

void CFlyTraceState::Exit()
{

}
