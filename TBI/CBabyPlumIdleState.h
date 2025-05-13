#pragma once
#include "CState.h"

class CBabyPlumIdleState :
    public CState
{
private:


public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CBabyPlumIdleState();
    ~CBabyPlumIdleState();
};

