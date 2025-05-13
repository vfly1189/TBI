#pragma once
#include "CState.h"
class CAttackState :
    public CState
{
private:


public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CAttackState();
    ~CAttackState();
};

