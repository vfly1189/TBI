#include "global.h"
#include "CBabyPlumDeadState.h"

#include "CTrophy.h"

#include "CMonster.h"
#include "CAnimation.h"
#include "CAnimator.h"

#include "CCollider.h"

#include "CTimeMgr.h"
#include "CCamera.h"

#include "CTear.h"
#include "CSpriteUI.h"
#include "CEntityMgr.h"
#include "Direct2DMgr.h"
#include "CPlayerMgr.h"
#include "CSoundMgr.h"
#include "CPlayer.h"

CBabyPlumDeadState::CBabyPlumDeadState()
	:CState(MON_STATE::DEAD)
{
}

CBabyPlumDeadState::~CBabyPlumDeadState()
{
}


void CBabyPlumDeadState::update()
{
	m_fAccTime += fDT;

	if (m_fAccTime > m_fExplosionDuration)
	{
		GetMonster()->GetAnimator()->PauseAllAnimations();
		GetMonster()->GetAnimator()->Play(L"dead_explosion", false, 1);

		CreateDeadTear();

		CSoundMgr::GetInstance()->Play(L"death_burst_large_0");

		DeleteObject(GetMonster());

		CreateTrophy();
		return;
	}	
}

void CBabyPlumDeadState::Enter()
{
	m_vPlayerPos = CPlayerMgr::GetInstance()->GetPlayer()->GetPos();
	m_vMonsterPos = GetMonster()->GetPos();

	GetMonster()->GetAnimator()->PauseAllAnimations();
	GetMonster()->GetAnimator()->Play(L"babyplum_dead", false, 1);
}


void CBabyPlumDeadState::Exit()
{

}

void CBabyPlumDeadState::CreateDeadTear()
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

void CBabyPlumDeadState::CreateTrophy()
{
	Vec2 vLookAt = CCamera::GetInstance()->GetLookAt();

	CTrophy* trophy = new CTrophy;
	trophy->SetObjType(GROUP_TYPE::TROPHY);
	trophy->SetScale(Vec2(32.f, 64.f) * 2.f);
	trophy->CreateCollider();
	trophy->GetCollider()->SetScale(Vec2(32.f, 40.f) * 2.f);
	trophy->SetPos(vLookAt);
	trophy->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"trophy"));
	CreateObject(trophy, GROUP_TYPE::TROPHY);
}
