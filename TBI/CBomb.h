#pragma once
#include "CItem.h"

enum class BOMB_STATE
{
    IGNITE,
    EXPLODE,
};

class CBomb :
    public CItem
{
private:
    float m_fAccTime;
    float m_fExplosionDuration = 3.f;
    float m_fAfterExplosionDuration = 0.6f;

    BOMB_STATE m_eState;

    bool m_bAnimationStopFlag = false;


public:
    BOMB_STATE GetBombState() { return m_eState; }

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
    void UpdateRenderScale();

public:
    CLONE(CBomb);

public:
    CBomb();
    ~CBomb();
};

