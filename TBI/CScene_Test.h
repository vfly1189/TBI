#pragma once
#include "CScene.h"

class Direct2DMgr;

class CScene_Test :
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
    void CreateMain(Vec2 _vResolution);
    void CreateDoor(Vec2 _vResolution);
    void CreateWall(Vec2 _vResolution);

public:
    CScene_Test();
    ~CScene_Test();
};

