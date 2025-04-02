#pragma once
#include "CObject.h"

class CItem :
    public CObject
{
private:
    Item            m_stItemStat;
 
public:
    void SetItemStat(Item _ItemStat) { m_stItemStat = _ItemStat; }
    Item GetItemStat() { return m_stItemStat; }

public:
    virtual void update() {}
    virtual void finalupdate() {}
    virtual void render(ID2D1HwndRenderTarget* _pRendeR) {}

public:
    CLONE(CItem);

public:
    CItem();
    ~CItem();
};

