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
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void CreateMain(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);

public:
    CScene_Test();
    ~CScene_Test();
};

