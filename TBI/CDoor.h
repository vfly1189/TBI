#pragma once
#include "CObject.h"
class CDoor :
    public CObject
{
private:
    int         m_iDoorDir;
 
public:
    void SetDoorDir(int _DoorDir) { m_iDoorDir = _DoorDir; }
    

public:
    int GetDoorDir() { return m_iDoorDir; }


public:
    virtual void start();
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

    CLONE(CDoor);

public:
    CDoor();
    ~CDoor();
};

