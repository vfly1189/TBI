#pragma once
#include "CState.h"

class CBabyPlumTrace :
    public CState
{
public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CBabyPlumTrace();
    ~CBabyPlumTrace();
};