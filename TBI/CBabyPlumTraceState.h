#pragma once
#include "CState.h"
class CBabyPlumTraceState:
	public CState
{
private:
    float m_fAccTime;
    float m_fAttackDuration;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CBabyPlumTraceState();
    ~CBabyPlumTraceState();
};

