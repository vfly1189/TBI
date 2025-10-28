#include "global.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCamera.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;


//�⺻ ���� �����ڴ� �׳� ���� ����. m_iID�� ����Ǹ�, �״�� ���� ���� ������ ��. 
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
	//�θ� ������Ʈ�� ��ġ�� ���󰣴�.
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}

void CCollider::render(ID2D1HwndRenderTarget* _pRender)
{
	// vFinalPos�� m_vScale�� ��ü�� ���� ��ġ�� ������ ���� ��Ÿ��.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vFinalPos);

	// ������ �簢�� ���
	D2D1_RECT_F rect = D2D1::RectF(
		vRenderPos.x - m_vScale.x / 2.f,
		vRenderPos.y - m_vScale.y / 2.f,
		vRenderPos.x + m_vScale.x / 2.f,
		vRenderPos.y + m_vScale.y / 2.f
	);

	// m_iCol ���� ���� ������ ����: m_iCol�� true�̸� ������, �ƴϸ� �ʷϻ�.
	D2D1_COLOR_F brushColor = m_iCol ? D2D1::ColorF(D2D1::ColorF::Red)
		: D2D1::ColorF(D2D1::ColorF::Green);

	// SolidColorBrush ���� (�� �귯�ô� �Ź� ����/�����ϴµ�, ���� ���ȴٸ� ����� ĳ���ϴ� ���� �����ϴ�.)
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(brushColor, &pBrush);
	if (SUCCEEDED(hr))
	{
		// �� �β� 1.0f�� �簢�� �ܰ����� �׸�.
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




