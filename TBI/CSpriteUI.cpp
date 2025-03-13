#include "global.h"
#include "CSpriteUI.h"
#include "CUI.h"

#include "CTimeMgr.h"
#include "CPathMgr.h"

#include "CObject.h"

#include "CAnimator.h"
#include "CAnimation.h"


CSpriteUI::CSpriteUI()
    :CUI(false)
    , m_colorRoundedNormal(D2D1::ColorF::Black)
    , m_colorRoundedMouseOn(D2D1::ColorF::Black)
    , m_colorFillMouseOn(D2D1::ColorF::Black)
    , m_colorFillNormal(D2D1::ColorF::Black)
    , m_bBackGround(false)
    , m_bIsMoving(false)
    , m_fMoveSpeed(100.f)
    , m_fMoveRange(50.f)
    , m_iMoveDirection(1)
    , m_vBaseOffset(0.f, 0.f)
    , m_fMovementOffset(0.f)
{

}

CSpriteUI::~CSpriteUI()
{

}

void CSpriteUI::finalupdate()
{
    CUI::finalupdate();

    if (GetParent())
    {
        Vec2 vParentPos = GetParent()->GetFinalPos();
        m_vFinalPos = vParentPos + m_vOffset;
    }
    else
    {
        m_vFinalPos = GetPos() + m_vOffset;
    }
}

void CSpriteUI::SetMovement(float _speed, float _range)
{
    m_bIsMoving = true;
    m_fMoveSpeed = _speed;
    m_fMoveRange = _range;

    // 현재 오프셋을 기본값으로 고정
    m_vBaseOffset = GetOffset();
    m_fMovementOffset = 0.f;
}

void CSpriteUI::update()
{ 
    if (m_bIsMoving)
    {
        //printf("%.f , %.f\n", m_vOffset.x, m_vOffset.y);
        // 프레임 독립적 이동 계산
        m_fMovementOffset += m_iMoveDirection * m_fMoveSpeed * fDT;

        // 방향 전환 체크
        if (abs(m_fMovementOffset) >= m_fMoveRange)
        {
            m_iMoveDirection *= -1;
        }

        // 기본 오프셋 + 이동 오프셋 적용
        SetOffset(m_vBaseOffset + Vec2(0.f, m_fMovementOffset));
    }

    if (GetAnimator() != nullptr)
        GetAnimator()->update();

    CUI::update();
}

void CSpriteUI::render(ID2D1HwndRenderTarget* _pRender)
{
    if (m_bBackGround)
    {
        Vec2 vOffset = GetOffset();
        Vec2 vPos = GetFinalPos();
        Vec2 vScale = GetScale();

        if (m_bCamAffected) {
            vPos = CCamera::GetInstance()->GetRenderPos(vPos);
        }

        // vPos와 vScale은 이미 계산된 좌표와 스케일 값입니다.
        D2D1_RECT_F rect = D2D1::RectF(
            vPos.x - vScale.x / 2.f,
            vPos.y - vScale.y / 2.f,
            vPos.x + vScale.x / 2.f,
            vPos.y + vScale.y / 2.f
        );
        D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
            D2D1::RectF(
                vPos.x - vScale.x / 2.f,
                vPos.y - vScale.y / 2.f,
                vPos.x + vScale.x / 2.f,
                vPos.y + vScale.y / 2.f), // 사각형의 좌표 (left, top, right, bottom)
            m_fradiusX, // X축 반지름 (radiusX)
            m_fradiusY  // Y축 반지름 (radiusY)
        );

        // 내부 색상 브러시 생성
        ID2D1SolidColorBrush* pFillBrush = nullptr;
        HRESULT hrFill = _pRender->CreateSolidColorBrush(
            m_bLbtnDown ? m_colorFillMouseOn : m_colorFillNormal, // 내부 색상
            &pFillBrush
        );

        // 외곽선 색상 브러시 생성
        ID2D1SolidColorBrush* pOutlineBrush = nullptr;
        HRESULT hrOutline = _pRender->CreateSolidColorBrush(
            m_bLbtnDown ? m_colorRoundedMouseOn : m_colorRoundedNormal, // 외곽선 색상 (검정색 예시)
            &pOutlineBrush
        );

        if (SUCCEEDED(hrFill) && SUCCEEDED(hrOutline)) {
            // 내부 채우기
            if (m_bIsRoundedRect) {
                _pRender->FillRoundedRectangle(roundedRect, pFillBrush);
            }
            else {
                _pRender->FillRectangle(rect, pFillBrush);
            }

            // 외곽선 그리기
            if (m_bIsRoundedRect) {
                _pRender->DrawRoundedRectangle(roundedRect, pOutlineBrush, 2.0f); // 두께 2.0f
            }
            else {
                _pRender->DrawRectangle(rect, pOutlineBrush, 2.0f); // 두께 2.0f
            }

            // 브러시 해제
            pFillBrush->Release();
            pOutlineBrush->Release();
        }
    }
    component_render(_pRender);
}

void CSpriteUI::MouseLbtnClicked()
{
    if (GetParent())
        GetParent()->MouseLbtnClicked();
}
