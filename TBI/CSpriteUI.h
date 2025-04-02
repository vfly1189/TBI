#pragma once
#include "CUI.h"

class CCollider;

class CSpriteUI
    : public CUI
{
private:
    //Vec2 m_vOffset;
    bool            m_bBackGround;

    //������
    bool    m_bIsMoving;       // ������ Ȱ��ȭ ����
    float   m_fMoveSpeed;      // �̵� �ӵ� (�ȼ�/��)
    float   m_fMoveRange;      // �̵� ���� (�ȼ�)
    int     m_iMoveDirection;  // �̵� ���� (1: �Ʒ�, -1: ��)

    // ���� ��� ���� ����
    Vec2    m_vBaseOffset;     // �⺻ ������ (��: (0, -115))
    float   m_fMovementOffset; // �̵� ������ (��: -20 ~ +20)


    //�Ϲݻ��� ���λ�, �ܰ�����
    D2D1::ColorF    m_colorFillNormal;
    D2D1::ColorF    m_colorRoundedNormal;
    //���콺�÷����� ���λ�, �ܰ�����
    D2D1::ColorF    m_colorFillMouseOn;
    D2D1::ColorF    m_colorRoundedMouseOn;

    bool            m_bIsRoundedRect;
    float           m_fradiusX;
    float           m_fradiusY;

public:
    void SetMovement(float _speed, float _range);

public:
    CLONE(CSpriteUI);

public:
    void SetBackGround(bool _bBackGround) { m_bBackGround = _bBackGround; }
    void SetBackGroundColor(D2D1::ColorF _fillMouseOn, D2D1::ColorF _roundedMouseOn
        , D2D1::ColorF _fillNormal, D2D1::ColorF _roundedNormal)
    {
        m_colorFillMouseOn = _fillMouseOn;
        m_colorRoundedMouseOn = _roundedMouseOn;
        m_colorFillNormal = _fillNormal;
        m_colorRoundedNormal = _roundedNormal;
    }
    void SetIsRound(bool _bIsRoundedRect, float _fradiusX, float _fradiusY)
    {
        m_bIsRoundedRect = _bIsRoundedRect;
        m_fradiusX = _fradiusX;
        m_fradiusY = _fradiusY;
    }

public:
    void update();
    void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void MouseLbtnClicked();    //Ŭ��. 

public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
public:
    CSpriteUI();
    virtual ~CSpriteUI();
};