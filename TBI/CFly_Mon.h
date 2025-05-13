#pragma once
#include "CMonster.h"

class CFly_Mon :
    public CMonster
{


public:
    virtual void update();


public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

public:
    ~CFly_Mon();

};

