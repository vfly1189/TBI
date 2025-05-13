#include "global.h"
#include "CSceneMgr.h"
#include "CPlayerMgr.h"

#include "CMonster.h"
#include "CPlayer.h"


#include "MapMgr.h"
#include "CSoundMgr.h"

#include "CAnimation.h"
#include "CAnimator.h"

#include "CFlyIdleState.h"




CFlyIdleState::CFlyIdleState()
	: CState(MON_STATE::IDLE)
{
}

CFlyIdleState::~CFlyIdleState()
{
}

void CFlyIdleState::update()
{
	CPlayer* pPlayer = (CPlayer*)CPlayerMgr::GetInstance()->GetPlayer();
	// ������ �ִ´�. 

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
		
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
	// �÷��̾��� ��ġ�� �޾ƿͼ� ������ ���� �ȿ� ������
	// ���� ���� ��ȯ. 
}

void CFlyIdleState::Enter()
{
}

void CFlyIdleState::Exit()
{
}