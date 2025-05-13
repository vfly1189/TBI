#include "global.h"
#include "CBabyPlumAttackState.h"

#include "CBaby_Plum.h"

#include "CTimeMgr.h"
#include "CPlayerMgr.h"
#include "CSoundMgr.h"
#include "CEntityMgr.h"
#include "Direct2DMgr.h"

#include "CMonster.h"
#include "CPlayer.h"
#include "CTear.h"

#include "CAnimation.h"
#include "CAnimator.h"


CBabyPlumAttackState::CBabyPlumAttackState()
	: CState(MON_STATE::ATTACK)
	, m_fDuration(4.f)
{

}

CBabyPlumAttackState::~CBabyPlumAttackState()
{

}

void CBabyPlumAttackState::update()
{
	if (GetMonster()->Getinfo().m_iCurHP <= 0)
	{
		ChangeAIState(GetAI(), MON_STATE::DEAD);
		return;
	}

	m_fAccTime += fDT;
	m_fTakeDownAccTime += fDT;
	m_fSpinAccTime += fDT;
	m_fBackBounceTearAccTime += fDT;
	
	if (m_eAttackType == BABYPLUM_ATTACK_TYPE::TAKE_DOWN)
	{
		if (m_fTakeDownAccTime > m_fTakeDownDuration && !m_bTakeDownDone)
		{
			m_bTakeDownDone = true;
			CreateTakeDownTear();
		}
	}
	else if (m_eAttackType == BABYPLUM_ATTACK_TYPE::SPIN)
	{
		Vec2 vMonDir = m_vPlayerPos - m_vMonsterPos;
		vMonDir.Normalize();

		m_vMonsterPos += vMonDir * 500.f * fDT;
		GetMonster()->SetPos(m_vMonsterPos);


		if (m_fSpinAccTime > m_fSpinDuration && !m_bSpinDone)
		{
			m_bSpinDone = true;
			CreateSpinTear();
		}
	}
	else if (m_eAttackType == BABYPLUM_ATTACK_TYPE::BACK_BOUNCE)
	{
		CBaby_Plum* test = (CBaby_Plum*)GetMonster();
		Vec2 vCurDir = test->GetDir();

		//printf("방향 %f %f\n", vCurDir.x, vCurDir.y);

		if (m_vPrevDir != vCurDir)
		{
			GetMonster()->GetAnimator()->PauseAllAnimations();
			
			if		(m_vPrevDir == Vec2(1, -1) && vCurDir == Vec2(1, 1)) GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_back_left", true, 1);
			else if (m_vPrevDir == Vec2(-1, -1) && vCurDir == Vec2(-1, 1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_back_right", true, 1);
			
			else if (m_vPrevDir == Vec2(1, 1) && vCurDir == Vec2(-1, 1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_back_right", true, 1);
			else if (m_vPrevDir == Vec2(1, -1) && vCurDir == Vec2(-1, -1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_front_right", true, 1);

			else if (m_vPrevDir == Vec2(1, 1) && vCurDir == Vec2(1, -1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_front_left", true, 1);
			else if (m_vPrevDir == Vec2(-1, 1) && vCurDir == Vec2(-1, -1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_front_right", true, 1);

			else if (m_vPrevDir == Vec2(-1, 1) && vCurDir == Vec2(1, 1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_back_left", true, 1);
			else if (m_vPrevDir == Vec2(-1, -1) && vCurDir == Vec2(1, -1))GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_front_left", true, 1);
		}
		if (m_fBackBounceTearAccTime > m_fBackBounceTearDuration)
		{
			m_fBackBounceTearAccTime = 0.f;
			CreateBackBounceTear();
		}

		// Calculate dynamic speed(200->800 over 7 seconds)
		float speed = 100.f + (800.f - 100.f) * (m_fAccTime / 7.f);
		if (speed > 800.f) speed = 800.f;  // Clamp maximum speed

		printf("speed : %f\n", speed);
		m_vMonsterPos += vCurDir * speed * fDT;


		GetMonster()->SetPos(m_vMonsterPos);

		m_vPrevDir = vCurDir;
	}
	

	if (m_fAccTime < m_fDuration)
	{
		return;
	}
	

	GetMonster()->GetAnimator()->PauseAllAnimations();
	GetMonster()->GetAnimator()->Play(L"babyplum_idle", true, 1);
	ChangeAIState(GetAI(), MON_STATE::IDLE);
}

void CBabyPlumAttackState::Enter()
{
	m_fAccTime = 0.f;
	m_fTakeDownAccTime = 0.f;
	m_bTakeDownDone = false;

	m_fSpinAccTime = 0.f;
	m_bSpinDone = false;

	m_fBackBounceTearAccTime = 0.f;

	m_vPlayerPos = CPlayerMgr::GetInstance()->GetPlayer()->GetPos();
	m_vMonsterPos = GetMonster()->GetPos();

	int rNum = rand() % 3;
	m_eAttackType = (BABYPLUM_ATTACK_TYPE)rNum;


	if (m_eAttackType == BABYPLUM_ATTACK_TYPE::TAKE_DOWN)
	{
		GetMonster()->GetAnimator()->Play(L"babyplum_attack_takedown", false, 1);
		m_fDuration = 1.f;
	}
	else if (m_eAttackType == BABYPLUM_ATTACK_TYPE::SPIN)
	{
		GetMonster()->GetAnimator()->Play(L"babyplum_attack_spin", false, 1);
		m_fDuration = 1.f;
	}

	else if (m_eAttackType == BABYPLUM_ATTACK_TYPE::BACK_BOUNCE)
	{
		CBaby_Plum* test = (CBaby_Plum*)GetMonster();
		test->SetDir(Vec2(1, -1));
		m_vPrevDir = Vec2(1, -1);
		GetMonster()->GetAnimator()->Play(L"babyplum_attack_backbounce_front_left", true, 1);
		m_fDuration = 7.f;
	}
}

void CBabyPlumAttackState::Exit()
{

}

void CBabyPlumAttackState::CreateTakeDownTear()
{
	for (int i = 0; i < 8; i++)
	{	
		float angle = i * (PI / 4); // 0, 45, 90, ..., 315도
		Vec2 vDir = { cos(angle), sin(angle) };

		CTear* attackTear = new CTear(m_vMonsterPos, GetMonster()->Getinfo().m_fAttRange, true);
		attackTear->SetObjType(GROUP_TYPE::TEAR);
		attackTear->SetPos(m_vMonsterPos);
		attackTear->SetSpeed(400.f);
		attackTear->SetName(L"Monster_Attack_Tear");
		attackTear->SetScale(Vec2(32.f, 32.f) * 2.f);
		attackTear->SetDir(vDir);
		attackTear->DeleteImages();
		//attackTear->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"monster_normal_tear"));
		attackTear->AddImage(CEntityMgr::GetInstance()->GetTearImage(5));
		CreateObject(attackTear, GROUP_TYPE::TEAR);
	}

	const int numDirections = 14;
	const float angleStep = 2 * PI / numDirections; // 한 방향당 각도

	for (int i = 0; i < 14; i++)
	{
		float angle = i * angleStep; // 0, angleStep, 2*angleStep, ...
		Vec2 vDir = { cos(angle), sin(angle) };

		CTear* attackTear = new CTear(m_vMonsterPos, GetMonster()->Getinfo().m_fAttRange, true);
		attackTear->SetObjType(GROUP_TYPE::TEAR);
		attackTear->SetPos(m_vMonsterPos);
		attackTear->SetSpeed(200.f);
		attackTear->SetName(L"Monster_Attack_Tear");
		attackTear->SetScale(Vec2(32.f, 32.f) * 2.f);
		attackTear->SetDir(vDir);
		attackTear->DeleteImages();
		//attackTear->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"monster_normal_tear"));
		attackTear->AddImage(CEntityMgr::GetInstance()->GetTearImage(5));
		CreateObject(attackTear, GROUP_TYPE::TEAR);
	}
}

void CBabyPlumAttackState::CreateSpinTear()
{
	const int numDirections = 12;
	const float angleStep = 2 * PI / numDirections; // 한 방향당 각도

	for (int i = 0; i < 12; i++)
	{
		float angle = i * angleStep; // 0, angleStep, 2*angleStep, ...
		Vec2 vDir = { cos(angle), sin(angle) };

		CTear* attackTear = new CTear(m_vMonsterPos, GetMonster()->Getinfo().m_fAttRange, true);
		attackTear->SetObjType(GROUP_TYPE::TEAR);
		attackTear->SetPos(m_vMonsterPos);
		attackTear->SetSpeed(300.f);
		attackTear->SetName(L"Monster_Attack_Tear");
		attackTear->SetScale(Vec2(32.f, 32.f) * 2.f);
		attackTear->SetDir(vDir);
		attackTear->DeleteImages();
		//attackTear->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"monster_normal_tear"));
		attackTear->AddImage(CEntityMgr::GetInstance()->GetTearImage(5));
		CreateObject(attackTear, GROUP_TYPE::TEAR);
	}

	CTear* attackTear = new CTear(m_vMonsterPos, GetMonster()->Getinfo().m_fAttRange, true);
	attackTear->SetObjType(GROUP_TYPE::TEAR);
	attackTear->SetPos(m_vMonsterPos);
	attackTear->SetSpeed(100.f);
	attackTear->SetName(L"Monster_Attack_Tear");
	attackTear->SetScale(Vec2(32.f, 32.f) * 2.f);
	attackTear->SetDir((CPlayerMgr::GetInstance()->GetPlayer()->GetPos() - m_vMonsterPos).Normalize());
	attackTear->DeleteImages();
	//attackTear->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"monster_normal_tear"));
	attackTear->AddImage(CEntityMgr::GetInstance()->GetTearImage(5));
	CreateObject(attackTear, GROUP_TYPE::TEAR);
}

void CBabyPlumAttackState::CreateBackBounceTear()
{
	CBaby_Plum* test = (CBaby_Plum*)GetMonster();
	Vec2 vCurDir = test->GetDir();

	CTear* attackTear = new CTear(m_vMonsterPos, GetMonster()->Getinfo().m_fAttRange, true);
	attackTear->SetObjType(GROUP_TYPE::TEAR);
	attackTear->SetPos(m_vMonsterPos);
	attackTear->SetSpeed(200.f);
	attackTear->SetName(L"Monster_Attack_Tear");
	attackTear->SetScale(Vec2(32.f, 32.f) * 2.f);

	
	// -15도 ~ 15도 사이의 랜덤 각도(도 단위)
	float fAngle = (rand() % 3001 - 1500) * 0.01f; // -15.00 ~ 15.00

	// 도(degree)를 라디안(radian)으로 변환
	float rad = fAngle * (3.14159265f / 180.0f);

	// -vCurDir을 rad만큼 회전
	Vec2 vRotDir = {
		-vCurDir.x * cosf(rad) - (-vCurDir.y) * sinf(rad),
		-vCurDir.x * sinf(rad) + (-vCurDir.y) * cosf(rad)
	};
	attackTear->SetDir(vRotDir);
	
	//attackTear->SetDir(-vCurDir);

	attackTear->DeleteImages();


	//attackTear->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"monster_normal_tear"));

	int tearNum = rand() % 4 + 5;
	attackTear->AddImage(CEntityMgr::GetInstance()->GetTearImage(tearNum));


	CreateObject(attackTear, GROUP_TYPE::TEAR);

	m_bBackBounceTearSound = !m_bBackBounceTearSound;
	if(m_bBackBounceTearSound) CSoundMgr::GetInstance()->Play(L"boss 2 bubbles", 0.5f);
	else  CSoundMgr::GetInstance()->Play(L"boss 2 bubbles 1", 0.5f);
}
