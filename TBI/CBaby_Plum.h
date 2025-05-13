#pragma once
#include "CMonster.h"
class CBaby_Plum :
    public CMonster
{
private:
    Vec2 m_vDirBackBounce;


public:
    Vec2 GetDir() { return m_vDirBackBounce; }
    void SetDir(Vec2 _vDir) { m_vDirBackBounce = _vDir; }

public:
    virtual void update();

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

public:
    CBaby_Plum();
    ~CBaby_Plum();
};

