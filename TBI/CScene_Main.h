#pragma once
#include "CScene.h"

class CObject;
class Direct2DMgr;
class CSpriteUI;

class CScene_Main :
    public CScene
{
private:
    CSpriteUI* Cursor;
    int m_iCursorPos;

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void CreateTitle(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);
    void CreateGameMenu(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);

public:
    CScene_Main();
    ~CScene_Main();
};

