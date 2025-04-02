#pragma once
#include "CObject.h"
class CTear :
    public CObject
{
private:
    Vec2 m_vStartPos;
    int  m_iSize;
    Vec2 m_vDir;
    float m_fSpeed;

    float m_fAccTime; //눈물 터지는 애니메이션 재생용 딜레이

    bool m_bDestroy; //벽과 충돌 시 삭제 예정이라고 플래그 세움 -> 애니메이션 재생 다 하기 전까지는 딜레이 걸어둠
    bool m_bStop;
public:
    void SetTearSize(int _size) { m_iSize = _size; }
    void SetDir(Vec2 _vDir)
    {
        m_vDir = _vDir;
        m_vDir.Normalize();
    }

    Vec2 GetDir() { return m_vDir; }

public:
    int GetTearSize() { return m_iSize; }

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

public:
    CLONE(CTear);

public:
    CTear(Vec2 _vStartPos);
    ~CTear();
};

