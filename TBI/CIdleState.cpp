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
	// ������ �ִ´�. 

	Vec2 vPlayerPos = pPlayer->GetPos();

	CMonster* pMonster = GetMonster();
	Vec2 vMonsterGridPos = pMonster->GetGridPos();
	Vec2 vMonsterPos = pMonster->GetPos();

	Vec2 vCurGridPos = MapMgr::GetInstance()->GetCurPos();

	printf("������ǥ : %f %f , ���� ��ǥ : %f %f\n", vMonsterGridPos.x, vMonsterGridPos.y, vCurGridPos.x, vCurGridPos.y);

	Vec2 vDiff = vPlayerPos - vMonsterPos;
	float fLen = vDiff.Length();

	if ((vMonsterGridPos.x == vCurGridPos.x && vMonsterGridPos.y == vCurGridPos.y)
		&& fLen < pMonster->Getinfo().m_fRecogRange)
	{
		printf("���º���\n");
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}

	// �÷��̾��� ��ġ�� �޾ƿͼ� ������ ���� �ȿ� ������
	// ���� ���� ��ȯ. 
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}