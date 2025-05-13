#pragma once
#include "CItem.h"
class CPickUpItem :
    public CItem
{
private:
    PICKUP_ITEM_TYPE m_ePickupType;


public:
    virtual void start();
    virtual void update();
    virtual void finalupdate();

    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

public:
    CLONE(CPickUpItem);

public:
    CPickUpItem(PICKUP_ITEM_TYPE _eType);
    ~CPickUpItem();
};

