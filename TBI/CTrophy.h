#pragma once
#include "CObject.h"
class CTrophy :
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

    CLONE(CTrophy);

public:
    CTrophy();
    ~CTrophy();
};

