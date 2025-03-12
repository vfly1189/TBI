#pragma once
#include "CObject.h"
class CPlayer :
    public CObject
{


public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    CLONE(CPlayer);

public:
    CPlayer();
    ~CPlayer();
};

