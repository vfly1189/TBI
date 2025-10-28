#include "global.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCamera.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;


//기본 복사 생성자는 그냥 얕은 복사. m_iID는 복사되면, 그대로 같은 값을 가지게 됨. 
CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
{
}

CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
	, m_bActive(_origin.m_bActive)
{
}

CCollider::~CCollider()
{
}

void CCollider::finalupdate()
{
	//부모 오브젝트의 위치를 따라간다.
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}

void CCollider::render(ID2D1HwndRenderTarget* _pRender)
{
	// vFinalPos와 m_vScale은 객체의 최종 위치와 스케일 값을 나타냄.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vFinalPos);

	// 목적지 사각형 계산
	D2D1_RECT_F rect = D2D1::RectF(
		vRenderPos.x - m_vScale.x / 2.f,
		vRenderPos.y - m_vScale.y / 2.f,
		vRenderPos.x + m_vScale.x / 2.f,
		vRenderPos.y + m_vScale.y / 2.f
	);

	// m_iCol 값에 따라 색상을 선택: m_iCol가 true이면 빨간색, 아니면 초록색.
	D2D1_COLOR_F brushColor = m_iCol ? D2D1::ColorF(D2D1::ColorF::Red)
		: D2D1::ColorF(D2D1::ColorF::Green);

	// SolidColorBrush 생성 (이 브러시는 매번 생성/해제하는데, 자주 사용된다면 멤버로 캐싱하는 것이 좋습니다.)
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(brushColor, &pBrush);
	if (SUCCEEDED(hr))
	{
		// 선 두께 1.0f로 사각형 외곽선을 그림.
		_pRender->DrawRectangle(rect, pBrush, 1.0f);
		pBrush->Release();
	}
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	m_pOwner->OnCollisionEnter(_pOther);
	m_iCol++;
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	m_pOwner->OnCollisionExit(_pOther);
	m_iCol--;
}




