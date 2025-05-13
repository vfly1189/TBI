#pragma once
#include "CState.h"
class CBabyPlumDeadState :
    public CState
{
private:
    float m_fAccTime;
    float m_fExplosionDuration = 0.6f;

    Vec2 m_vPlayerPos;
    Vec2 m_vMonsterPos;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    void CreateDeadTear();


public:
    CBabyPlumDeadState();
    ~CBabyPlumDeadState();
};

