#include "global.h"

#include "CFly_Mon.h"
#include "CHorf_Mon.h"
#include "CBaby_Plum.h"

#include "CMonFactory.h"
#include "AI.h"
#include "CIdleState.h"
#include "Direct2DMgr.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTraceState.h"

#include "CFlyIdleState.h"
#include "CFlyTraceState.h"
#include "CFlyDeadState.h"

#include "CHorfIdleState.h"
#include "CHorfDeadState.h"
#include "CHorfAttackState.h"

#include "CBabyPlumIdleState.h"
#include "CBabyPlumAttackState.h"
#include "CBabyPlumTraceState.h"
#include "CBabyPlumDeadState.h"

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos, Vec2 _vGridPos, int option)
{
	CMonster* pMon = nullptr;
	switch (_eType) 
	{
		case MON_TYPE::FLY:
		{
			pMon = new CFly_Mon;
			pMon->SetObjType(GROUP_TYPE::ENTITY);
			pMon->SetPos(_vPos);
			pMon->SetGridPos(_vGridPos);
			pMon->SetName(L"Fly_Mon");
			pMon->SetScale(Vec2(32.f, 32.f));
			pMon->SetRenderScale(pMon->GetScale() * 1.5f);

			tMonInfo info = {};
			info.m_eMonType = _eType;
			info.m_fAtt = 1;
			info.m_fAttRange = 50.f;
			info.m_fRecogRange = 99999.f;
			info.m_iCurHP = 11;
			info.m_fSpeed = 40.f;
			pMon->SetMonInfo(info);

			AI* pAI = new AI;
			pAI->AddState(new CFlyDeadState);
			pAI->AddState(new CFlyIdleState);
			pAI->AddState(new CFlyTraceState);
			pAI->SetCurState(MON_STATE::IDLE);
			pMon->SetAI(pAI);
		
		
			pMon->GetAnimator()->CreateAnimation(L"fly_dead_anim", Direct2DMgr::GetInstance()->GetStoredBitmap(L"fly_dead")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.03f, 11, false, Vec2(0.f, 0.f));

			if (option == 0)
			{
				pMon->GetAnimator()->CreateAnimation(L"normal_fly_anim", Direct2DMgr::GetInstance()->GetStoredBitmap(L"normal_fly")
					, Vec2(0.f, 0.f), Vec2(32.f,32.f), Vec2(32.f, 0.f), 0.03f, 2, true, Vec2(0.f, 0.f));
				pMon->GetAnimator()->Play(L"normal_fly_anim", true, 1);
			}
			else
			{
				pMon->GetAnimator()->CreateAnimation(L"attack_fly_anim", Direct2DMgr::GetInstance()->GetStoredBitmap(L"attack_fly")
					, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.03f, 4, true, Vec2(0.f, 0.f));
				pMon->GetAnimator()->Play(L"attack_fly_anim", true, 1);
			}
		}
		break;

		case MON_TYPE::HORF:
		{
			pMon = new CHorf_Mon;
			pMon->SetObjType(GROUP_TYPE::ENTITY);
			pMon->SetPos(_vPos);
			pMon->SetGridPos(_vGridPos);
			pMon->SetName(L"Horf_Mon");
			pMon->SetScale(Vec2(32.f, 32.f));
			pMon->SetRenderScale(pMon->GetScale() * 1.5f);

			tMonInfo info = {};
			info.m_eMonType = _eType;
			info.m_fAtt = 1;
			info.m_fAttRange = 800.f;
			info.m_fRecogRange = 200.f;
			info.m_iCurHP = 20;
			info.m_fSpeed = 0.f;
			pMon->SetMonInfo(info);

			AI* pAI = new AI;
			pAI->AddState(new CHorfIdleState);
			pAI->AddState(new CHorfAttackState);
			pAI->AddState(new CHorfDeadState);
			pAI->SetCurState(MON_STATE::IDLE);
			pMon->SetAI(pAI);

			//기본애니메이션//
			pMon->GetAnimator()->CreateAnimation(L"horf_idle_anim", Direct2DMgr::GetInstance()->GetStoredBitmap(L"horf")
				, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), 1.f, 1, true, Vec2(0.f, 0.f));
			pMon->GetAnimator()->Play(L"horf_idle_anim", true, 1);

			//공격 모션
			pMon->GetAnimator()->CreateAnimation(L"horf_attack_anim", Direct2DMgr::GetInstance()->GetStoredBitmap(L"horf")
				, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.5f, 4, true, Vec2(0.f, 0.f));
		
			//죽는모션
			pMon->GetAnimator()->CreateAnimation(L"mon_dead_anim", Direct2DMgr::GetInstance()->GetStoredBitmap(L"mon_dead")
				, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f), 0.03f, 12, false, Vec2(0.f, 0.f));
		}
		break;
		case MON_TYPE::BABY_PLUM:
		{
			pMon = new CBaby_Plum;
			pMon->SetObjType(GROUP_TYPE::ENTITY);
			pMon->SetPos(_vPos);
			pMon->SetGridPos(_vGridPos);
			pMon->SetName(L"Baby_Plum_Boss");
			pMon->SetScale(Vec2(64.f, 64.f) * 2.f);
			pMon->SetRenderScale(pMon->GetScale() * 2.f);

			tMonInfo info = {};
			info.m_eMonType = _eType;
			info.m_fAtt = 1;
			info.m_fAttRange = 800.f;
			info.m_fRecogRange = 99999.f;
			info.m_iMaxHP = 100;
			info.m_iCurHP = 100;
			info.m_fSpeed = 50.f;
			pMon->SetMonInfo(info);

			AI* pAI = new AI;
			pAI->AddState(new CBabyPlumIdleState);
			pAI->AddState(new CBabyPlumTraceState);
			pAI->AddState(new CBabyPlumAttackState);
			pAI->AddState(new CBabyPlumDeadState);
			pAI->SetCurState(MON_STATE::IDLE);
			pMon->SetAI(pAI);

			//기본애니메이션//
			pMon->GetAnimator()->CreateAnimation(L"babyplum_idle", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_idle")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.05f, 2, true, Vec2(0.f, 0.f));
			
			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_takedown", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_takedown")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.2f, 5, true, Vec2(0.f, 0.f));
			vector<tAnimFrm>& m_vecFrm = pMon->GetAnimator()->GetAnim(L"babyplum_attack_takedown")->GetAllFrame();
			m_vecFrm[0].vOffset = Vec2(0.f, -50.f);
			m_vecFrm[1].vOffset = Vec2(0.f, -50.f);

			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_spin", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_spin")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 1.f / 11.f, 11, true, Vec2(0.f, 0.f));


			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_backbounce_start", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_backbounce_start")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 2, true, Vec2(0.f, 0.f));
			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_backbounce_end", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_backbounce_end")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 2, true, Vec2(0.f, 0.f));
			
			
			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_backbounce_front_left", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_backbounce_front_left")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 2, true, Vec2(0.f, 0.f));
			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_backbounce_front_right", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_backbounce_front_right")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 2, true, Vec2(0.f, 0.f));
			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_backbounce_back_left", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_backbounce_back_left")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 2, true, Vec2(0.f, 0.f));
			pMon->GetAnimator()->CreateAnimation(L"babyplum_attack_backbounce_back_right", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_attack_backbounce_back_right")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 2, true, Vec2(0.f, 0.f));

			//사망모션
			pMon->GetAnimator()->CreateAnimation(L"babyplum_dead", Direct2DMgr::GetInstance()->GetStoredBitmap(L"babyplum_dead")
				, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.2f, 3, true, Vec2(0.f, 0.f));




			pMon->GetAnimator()->Play(L"babyplum_idle", true, 1);

		}
		break;

	}
	

	assert(pMon);
	return pMon;
}


