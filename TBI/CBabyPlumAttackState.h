#pragma once
#include "CState.h"

enum class BABYPLUM_ATTACK_TYPE
{
    TAKE_DOWN,
    SPIN,
    BACK_BOUNCE,
};

class CBabyPlumAttackState :
	public CState
{
private:
    float m_fAccTime;
    float m_fDuration;
    Vec2 m_vPlayerPos;
    Vec2 m_vMonsterPos;

    Vec2 m_vPrevDir;

    BABYPLUM_ATTACK_TYPE m_eAttackType;

private:
    float m_fTakeDownAccTime;
    float m_fTakeDownDuration = 0.4f;
    bool  m_bTakeDownDone = false;

private:
    float m_fSpinAccTime;
    float m_fSpinDuration = 0.1f;
    bool m_bSpinDone = false;

private:
    float m_fBackBounceTearAccTime;
    float m_fBackBounceTearDuration = 0.075f;
    float m_bBackBounceTearSound = false;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    void CreateTakeDownTear();
    void CreateSpinTear();
    void CreateBackBounceTear();

public:
    CBabyPlumAttackState();
    ~CBabyPlumAttackState();

};

