#pragma once
#include "CScene.h"

class Direct2DMgr;

class CScene_Fight :
    public CScene
{
private:
    Direct2DMgr* pD2DMgr;


public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    CScene_Fight();
    ~CScene_Fight();
};

