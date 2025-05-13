#pragma once
#include "CState.h"
class CHorfDeadState :
    public CState
{
private:
    float m_fAccTime;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

private:
    void CreatePickupItem(Vec2 _vPos);

public:
    CHorfDeadState();
    ~CHorfDeadState();
};

