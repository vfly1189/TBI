#pragma once
#include "CState.h"

class CBabyPlumIdleState :
    public CState
{
private:
    float m_fAccTime;
    float m_fStartAnimDuration = 1.0f;

    bool m_bStartAnimTrigger = false;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CBabyPlumIdleState();
    ~CBabyPlumIdleState();
};

