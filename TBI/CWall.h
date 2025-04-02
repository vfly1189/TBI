#pragma once
#include "CObject.h"
class CWall :
    public CObject
{

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

