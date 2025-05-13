#pragma once
#include "CState.h"
class CHorfIdleState :
    public CState
{
private:


public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CHorfIdleState();
    ~CHorfIdleState();
};

