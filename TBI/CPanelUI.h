#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{

private:
    Vec2            m_vDragStart;
    bool            m_bCanMove;

    float           m_fradiusX;
    float           m_fradiusY;

    //MouseOn�϶��� �����϶� ��
    D2D1::ColorF    m_colorMouseOn;         //0~1 ����ȭ ���Ѿߵ�
    float           m_fMouseOnAlpha;        //���콺on�϶� alpha��

    D2D1::ColorF    m_colorNormal;
    float           m_fNormalAlpha;         //�Ϲݻ����ϋ� alpha��
public:
    void SetCanMove(bool _b) { m_bCanMove = _b; }

public:
    void SetRadius(float _fradiusX, float _fradiusY)
    {
        m_fradiusX = _fradiusX;
        m_fradiusY = _fradiusY;
    }
    void SetColor(D2D1::ColorF _mouseOn, D2D1::ColorF _normal)
    {
        m_colorMouseOn = _mouseOn;
        m_colorNormal = _normal;
    }
    void SetNormalAlpha(float _v) { m_fNormalAlpha = _v; }
    void SetMouseOnAlpha(float _v) { m_fMouseOnAlpha = _v; }


public:
    virtual void update();
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    CLONE(CPanelUI)
public:
    CPanelUI();
    virtual ~CPanelUI();
};