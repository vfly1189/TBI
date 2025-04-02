#pragma once
#include "CItem.h"

class CSpriteUI;
class CObject;


class CCollectiblesItem :
    public CItem
{
private:
    COLLECTIBLES_ITEM_TYPE  m_eItemType;
    Item                    m_stItemInfo;

    vector<CObject*>        m_vecObjects;

public:
    Item GetItemInfo() { return m_stItemInfo; }

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
    CLONE(CCollectiblesItem);

public:
    CCollectiblesItem(Vec2 _vPos, int _iNum);
    ~CCollectiblesItem();
};

