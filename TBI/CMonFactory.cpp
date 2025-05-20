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

// ��� ����
namespace MonsterConstants {
    // ���� ũ�� ���
    constexpr float FLY_SCALE = 32.0f;
    constexpr float HORF_SCALE = 32.0f;
    constexpr float BABY_PLUM_SCALE = 128.0f; // 64.0f * 2.0f

    // ������ �¼�
    constexpr float FLY_RENDER_SCALE_MULT = 1.5f;
    constexpr float HORF_RENDER_SCALE_MULT = 1.5f;
    constexpr float BABY_PLUM_RENDER_SCALE_MULT = 2.0f;

    // �ִϸ��̼� ������ �ð�
    constexpr float NORMAL_ANIM_TIME = 0.03f;
    constexpr float ATTACK_ANIM_TIME = 0.2f;
    constexpr float IDLE_ANIM_TIME = 1.0f;
    constexpr float SPIN_ANIM_TIME = 1.0f / 11.0f;
}

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos, Vec2 _vGridPos, int option)
{
    CMonster* pMon = nullptr;

    // ���� ��ü ���� �� ���� �Ӽ� �ʱ�ȭ
    switch (_eType)
    {
    case MON_TYPE::FLY:
        pMon = CreateFlyMonster(_vPos, _vGridPos, option);
        break;

    case MON_TYPE::HORF:
        pMon = CreateHorfMonster(_vPos, _vGridPos);
        break;

    case MON_TYPE::BABY_PLUM:
        pMon = CreateBabyPlumMonster(_vPos, _vGridPos);
        break;
    }

    assert(pMon);
    return pMon;
}

// Fly ���� ���� �Լ�
CMonster* CMonFactory::CreateFlyMonster(Vec2 _vPos, Vec2 _vGridPos, int option)
{
    using namespace MonsterConstants;

    CFly_Mon* pMon = new CFly_Mon;
    InitializeBaseMonster(pMon, _vPos, _vGridPos, L"Fly_Mon", Vec2(FLY_SCALE, FLY_SCALE), FLY_RENDER_SCALE_MULT);

    // ���� ���� ����
    tMonInfo info = {};
    info.m_eMonType = MON_TYPE::FLY;
    info.m_fAtt = 1;
    info.m_fAttRange = 50.f;
    info.m_fRecogRange = 99999.f;
    info.m_iCurHP = 11;
    info.m_fSpeed = 40.f;
    pMon->SetMonInfo(info);

    // AI ����
    AI* pAI = new AI;
    pAI->AddState(new CFlyDeadState);
    pAI->AddState(new CFlyIdleState);
    pAI->AddState(new CFlyTraceState);
    pAI->SetCurState(MON_STATE::IDLE);
    pMon->SetAI(pAI);

    // �ִϸ��̼� ����
    auto pAnimator = pMon->GetAnimator();
    CreateAnimation(pAnimator, L"fly_dead_anim", L"fly_dead",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        NORMAL_ANIM_TIME, 11, false);

    if (option == 0)
    {
        CreateAnimation(pAnimator, L"normal_fly_anim", L"normal_fly",
            Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f),
            NORMAL_ANIM_TIME, 2, true);
        pAnimator->Play(L"normal_fly_anim", true, 1);
    }
    else
    {
        CreateAnimation(pAnimator, L"attack_fly_anim", L"attack_fly",
            Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f),
            NORMAL_ANIM_TIME, 4, true);
        pAnimator->Play(L"attack_fly_anim", true, 1);
    }

    return pMon;
}

// Horf ���� ���� �Լ�
CMonster* CMonFactory::CreateHorfMonster(Vec2 _vPos, Vec2 _vGridPos)
{
    using namespace MonsterConstants;

    CHorf_Mon* pMon = new CHorf_Mon;
    InitializeBaseMonster(pMon, _vPos, _vGridPos, L"Horf_Mon", Vec2(HORF_SCALE, HORF_SCALE), HORF_RENDER_SCALE_MULT);

    // ���� ���� ����
    tMonInfo info = {};
    info.m_eMonType = MON_TYPE::HORF;
    info.m_fAtt = 1;
    info.m_fAttRange = 800.f;
    info.m_fRecogRange = 200.f;
    info.m_iCurHP = 20;
    info.m_fSpeed = 0.f;
    pMon->SetMonInfo(info);

    // AI ����
    AI* pAI = new AI;
    pAI->AddState(new CHorfIdleState);
    pAI->AddState(new CHorfAttackState);
    pAI->AddState(new CHorfDeadState);
    pAI->SetCurState(MON_STATE::IDLE);
    pMon->SetAI(pAI);

    // �ִϸ��̼� ����
    auto pAnimator = pMon->GetAnimator();

    // �⺻ �ִϸ��̼�
    CreateAnimation(pAnimator, L"horf_idle_anim", L"horf",
        Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f),
        IDLE_ANIM_TIME, 1, true);
    pAnimator->Play(L"horf_idle_anim", true, 1);

    // ���� ���
    CreateAnimation(pAnimator, L"horf_attack_anim", L"horf",
        Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f),
        ATTACK_ANIM_TIME, 4, true);

    // �״� ���
    CreateAnimation(pAnimator, L"mon_dead_anim", L"mon_dead",
        Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(32.f, 0.f),
        NORMAL_ANIM_TIME, 12, false);

    return pMon;
}

// Baby Plum ���� ���� �Լ�
CMonster* CMonFactory::CreateBabyPlumMonster(Vec2 _vPos, Vec2 _vGridPos)
{
    using namespace MonsterConstants;

    CBaby_Plum* pMon = new CBaby_Plum;
    InitializeBaseMonster(pMon, _vPos, _vGridPos, L"Baby_Plum_Boss", Vec2(BABY_PLUM_SCALE, BABY_PLUM_SCALE), BABY_PLUM_RENDER_SCALE_MULT);

    // ���� ���� ����
    tMonInfo info = {};
    info.m_eMonType = MON_TYPE::BABY_PLUM;
    info.m_fAtt = 1;
    info.m_fAttRange = 800.f;
    info.m_fRecogRange = 99999.f;
    info.m_iMaxHP = 1000;
    info.m_iCurHP = 1000;
    info.m_fSpeed = 50.f;
    pMon->SetMonInfo(info);

    // AI ����
    AI* pAI = new AI;
    pAI->AddState(new CBabyPlumIdleState);
    pAI->AddState(new CBabyPlumTraceState);
    pAI->AddState(new CBabyPlumAttackState);
    pAI->AddState(new CBabyPlumDeadState);
    pAI->SetCurState(MON_STATE::IDLE);
    pMon->SetAI(pAI);

    // �ִϸ��̼� ����
    auto pAnimator = pMon->GetAnimator();

    // �⺻ �ִϸ��̼�
    CreateAnimation(pAnimator, L"babyplum_idle", L"babyplum_idle",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        0.05f, 2, true);

    // ���� �ִϸ��̼ǵ�
    CreateAnimation(pAnimator, L"babyplum_attack_takedown", L"babyplum_attack_takedown",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        ATTACK_ANIM_TIME, 5, true);

    // Ư�� ������ ������ ����
    vector<tAnimFrm>& m_vecFrm = pAnimator->GetAnim(L"babyplum_attack_takedown")->GetAllFrame();
    m_vecFrm[0].vOffset = Vec2(0.f, -50.f);
    m_vecFrm[1].vOffset = Vec2(0.f, -50.f);

    CreateAnimation(pAnimator, L"babyplum_attack_spin", L"babyplum_attack_spin",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        SPIN_ANIM_TIME, 11, true);

    // �ٿ �ִϸ��̼ǵ�
    CreateAnimation(pAnimator, L"babyplum_attack_backbounce_start", L"babyplum_attack_backbounce_start",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        0.1f, 2, true);
    CreateAnimation(pAnimator, L"babyplum_attack_backbounce_end", L"babyplum_attack_backbounce_end",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        0.1f, 2, true);

    // ���⺰ �ٿ �ִϸ��̼�
    const float bounceAnimTime = 0.1f;
    const Vec2 bounceFrameSize(64.f, 64.f);
    const Vec2 bounceFrameStep(64.f, 0.f);
    const int bounceFrameCount = 2;

    std::vector<std::wstring> bounceDirections = {
        L"front_left", L"front_right", L"back_left", L"back_right"
    };

    for (const auto& direction : bounceDirections) {
        std::wstring animName = L"babyplum_attack_backbounce_" + direction;
        std::wstring bitmapName = animName;

        CreateAnimation(pAnimator, animName, bitmapName,
            Vec2(0.f, 0.f), bounceFrameSize, bounceFrameStep,
            bounceAnimTime, bounceFrameCount, true);
    }

    // ��� ���
    CreateAnimation(pAnimator, L"babyplum_dead", L"babyplum_dead",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        ATTACK_ANIM_TIME, 3, true);

    // ���� ���
    CreateAnimation(pAnimator, L"babyplum_start", L"babyplum_start",
        Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f),
        ATTACK_ANIM_TIME, 5, false);

    return pMon;
}

// ���� ���� �ʱ�ȭ �Լ�
void CMonFactory::InitializeBaseMonster(CMonster* pMon, Vec2 _vPos, Vec2 _vGridPos,
    const wstring& name, Vec2 scale, float renderScaleMult)
{
    pMon->SetObjType(GROUP_TYPE::ENTITY);
    pMon->SetPos(_vPos);
    pMon->SetGridPos(_vGridPos);
    pMon->SetName(name);
    pMon->SetScale(scale);
    pMon->SetRenderScale(scale * renderScaleMult);
}

// �ִϸ��̼� ���� ����ȭ �Լ�
void CMonFactory::CreateAnimation(CAnimator* pAnimator, const wstring& animName, const wstring& bitmapName,
    Vec2 startPos, Vec2 frameSize, Vec2 step, float duration, int frameCount, bool loop,
    Vec2 offset)
{
    pAnimator->CreateAnimation(animName, Direct2DMgr::GetInstance()->GetStoredBitmap(bitmapName),
        startPos, frameSize, step, duration, frameCount, loop, offset);
}