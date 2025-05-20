#include "global.h"
#include "CBabyPlumIdleState.h"

#include "CSceneMgr.h"
#include "CPlayerMgr.h"

#include "CMonster.h"
#include "CPlayer.h"

#include "MapMgr.h"
#include "CTimeMgr.h"
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
	CPlayer* pPlayer = (CPlayer*)CPlayerMgr::GetInstance()->GetPlayer();
	// ������ �ִ´�. 

	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();


	Vec2 vDiff = vPlayerPos - vMonsterPos;

	if ((vMonsterGridPos.x == vCurGridPos.x && vMonsterGridPos.y == vCurGridPos.y))
	{
		if (!m_bStartAnimTrigger)
		{
			m_bStartAnimTrigger = true;
			GetMonster()->GetAnimator()->Play(L"babyplum_start", false, 1);
		}
		m_fAccTime += fDT;
	}
	if (GetMonster()->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}

	if (m_fAccTime < m_fStartAnimDuration)
	{
		return;
	}
	
	float fLen = vDiff.Length();

	if ((vMonsterGridPos.x == vCurGridPos.x && vMonsterGridPos.y == vCurGridPos.y)
		&& fLen < pMonster->Getinfo().m_fRecogRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
	// �÷��̾��� ��ġ�� �޾ƿͼ� ������ ���� �ȿ� ������
	// ���� ���� ��ȯ. 
}

void CBabyPlumIdleState::Enter()
{
	m_bStartAnimTrigger = false;
}

void CBabyPlumIdleState::Exit()
{

}

