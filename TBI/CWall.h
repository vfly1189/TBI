#pragma once
#include "CObject.h"
class CWall :
    public CObject
{
private:
    int m_iPos; // 0: 위 , 1: 오른, 2: 아래, 3: 왼

public:
    int GetWallPos() { return m_iPos; }
    void SetWallPos(int _iPos) { m_iPos = _iPos; }

public:
    virtual void start();
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);


    CLONE(CWall);

public:
    CWall();
    ~CWall();
};

