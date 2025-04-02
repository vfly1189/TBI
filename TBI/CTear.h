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

    float m_fAccTime; //���� ������ �ִϸ��̼� ����� ������

    bool m_bDestroy; //���� �浹 �� ���� �����̶�� �÷��� ���� -> �ִϸ��̼� ��� �� �ϱ� �������� ������ �ɾ��
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

