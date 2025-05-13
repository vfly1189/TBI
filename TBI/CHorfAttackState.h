#pragma once
#include "CState.h"
class CHorfAttackState :
    public CState
{
private:
    float m_fShootAcc;
    float m_fShootDuration;

    Vec2 m_vPlayerPos;
    Vec2 m_vMonsterPos;
    Vec2 m_vTearDir;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    void CreateTear();

public:
    CHorfAttackState();
    ~CHorfAttackState();
};

