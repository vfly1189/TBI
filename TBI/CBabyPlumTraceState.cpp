#include "global.h"
#include "CBabyPlumTraceState.h"

#include "CPlayer.h"
#include "CSoundMgr.h"
#include "CPlayerMgr.h"
#include "CMonster.h"
#include "MapMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"

#include "CAnimation.h"
#include "CAnimator.h"

#include "CScene_Fight.h"

CBabyPlumTraceState::CBabyPlumTraceState()
	: CState(MON_STATE::TRACE)
	, m_fAttackDuration(3.f)
{
}

CBabyPlumTraceState::~CBabyPlumTraceState()
{
}


void CBabyPlumTraceState::update()
{
	if (GetMonster()->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}

	if (dynamic_cast<CScene_Fight*>(CSceneMgr::GetInstance()->GetCurScene())->IsGameOver() == true)
	{
		return;
	}

	m_fAccTime += fDT;

	if (m_fAccTime > m_fAttackDuration)
	{
		m_fAccTime = 0.f;
		GetMonster()->GetAnimator()->PauseAllAnimations();
		ChangeAIState(GetAI(), MON_STATE::ATTACK);
		return;
	}

	//타겟팅 된 Player를 쫒아간다. 
	CPlayer* pPlayer = CPlayerMgr::GetInstance()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterPos = pMonster->GetPos();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();

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

void CBabyPlumTraceState::Enter()
{
	GetMonster()->GetAnimator()->PauseAllAnimations();
	GetMonster()->GetAnimator()->Play(L"babyplum_idle", true, 1);

	m_fAccTime = 0.f;
}

void CBabyPlumTraceState::Exit()
{
}

