#pragma once
#include "CObject.h"

class CellMap;

class CDoor :
    public CObject
{
private:
    int         m_iDoorDir;
    CellMap*    m_pOwner;

    bool        m_bIsOpen;          // true = ¿­¸² , false = ´ÝÈû
    ROOM_INFO   m_eRoomType;

public:
    void SetDoorDir(int _DoorDir) { m_iDoorDir = _DoorDir; }
    void SetOwner(CellMap* _Owner) { m_pOwner = _Owner; }
    void SetClose() { m_bIsOpen = false; }
    void SetOpen() { m_bIsOpen = true; }
    void SetRoomType(ROOM_INFO _eType) { m_eRoomType = _eType; }

public:
    int GetDoorDir() { return m_iDoorDir; }
    CellMap* GetOwner() { return m_pOwner; }

public:
    bool IsDoorOpen() { return m_bIsOpen; }

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

