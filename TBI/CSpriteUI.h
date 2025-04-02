#pragma once
#include "CUI.h"

class CCollider;

class CSpriteUI
    : public CUI
{
private:
    //Vec2 m_vOffset;
    bool            m_bBackGround;

    //움직임
    bool    m_bIsMoving;       // 움직임 활성화 여부
    float   m_fMoveSpeed;      // 이동 속도 (픽셀/초)
    float   m_fMoveRange;      // 이동 범위 (픽셀)
    int     m_iMoveDirection;  // 이동 방향 (1: 아래, -1: 위)

    // 기존 멤버 변수 수정
    Vec2    m_vBaseOffset;     // 기본 오프셋 (예: (0, -115))
    float   m_fMovementOffset; // 이동 오프셋 (예: -20 ~ +20)


    //일반상태 내부색, 외곽선색
    D2D1::ColorF    m_colorFillNormal;
    D2D1::ColorF    m_colorRoundedNormal;
    //마우스올렸을때 내부색, 외곽선색
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
    virtual void MouseLbtnClicked();    //클릭. 

public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
public:
    CSpriteUI();
    virtual ~CSpriteUI();
};