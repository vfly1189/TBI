#include "global.h"
#include "CHorfAttackState.h"

#include "CPlayer.h"
#include "CPlayerMgr.h"
#include "CMonster.h"
#include "CTear.h"

#include "MapMgr.h"
#include "CTimeMgr.h"
#include "Direct2DMgr.h"
#include "CSoundMgr.h"
#include "CScene_Fight.h"
#include "CSceneMgr.h"

#include "CAnimation.h"
#include "CAnimator.h"

CHorfAttackState::CHorfAttackState()
	:CState(MON_STATE::ATTACK)
	, m_fShootDuration(0.8f)
{

}

CHorfAttackState::~CHorfAttackState()
{

}

void CHorfAttackState::update()
{
	if (GetMonster()->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}

	if (dynamic_cast<CScene_Fight*>(CSceneMgr::GetInstance()->GetCurScene())->IsGameOver() == true)
	{
		GetMonster()->GetAnimator()->PauseAllAnimations();
		GetMonster()->GetAnimator()->Play(L"horf_idle_anim", true, 1);
		return;
	}


	m_fShootAcc += fDT;

	if (m_fShootAcc < m_fShootDuration)
	{
    //    printf("°ø°Ý ÄðÅ¸ÀÓ : %f\n", m_fShootAcc);
		return;
	}

    CreateTear();

    GetMonster()->GetAnimator()->PauseAllAnimations();
    GetMonster()->GetAnimator()->Play(L"horf_idle_anim", true, 1);

	ChangeAIState(GetAI(), MON_STATE::IDLE);
}

void CHorfAttackState::Enter()
{
	m_fShootAcc = 0.f;

	m_vPlayerPos = CPlayerMgr::GetInstance()->GetPlayer()->GetPos();
	m_vMonsterPos = GetMonster()->GetPos();
	m_vTearDir = (m_vPlayerPos - m_vMonsterPos).Normalize();
}

void CHorfAttackState::Exit()
{
	m_fShootAcc = 0.f;
}

void CHorfAttackState::CreateTear()
{
	CTear* attackTear = new CTear(m_vMonsterPos, GetMonster()->Getinfo().m_fAttRange, true);
    attackTear->SetObjType(GROUP_TYPE::TEAR);
    attackTear->SetPos(m_vMonsterPos);
    attackTear->SetName(L"Monster_Attack_Tear");
    attackTear->SetScale(Vec2(32.f, 32.f) * 2.f);
    attackTear->SetDir((CPlayerMgr::GetInstance()->GetPlayer()->GetPos() - m_vMonsterPos).Normalize());
	attackTear->DeleteImages();
	attackTear->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"monster_normal_tear"));
    CreateObject(attackTear, GROUP_TYPE::TEAR);

	CSoundMgr::GetInstance()->Play(L"shakey kid roar", 0.2f);
}
