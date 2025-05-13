#pragma once
#include "CState.h"
class CFlyIdleState :
    public CState
{
private:


public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CFlyIdleState();
    ~CFlyIdleState();
};

