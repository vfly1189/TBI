#pragma once
#include "CScene.h"
class CScene_Test :
    public CScene
{
public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    CScene_Test();
    ~CScene_Test();
};

