#pragma once
#include "CScene.h"
class CScene_Test :
    public CScene
{
public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    CScene_Test();
    ~CScene_Test();
};

