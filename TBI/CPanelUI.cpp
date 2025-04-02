#include "global.h"
#include "CPanelUI.h"
#include "CKeyMgr.h"
#include "CUI.h"


CPanelUI::CPanelUI()
	: CUI(false)
	, m_bCanMove(true)
	, m_fradiusX(10.f)
	, m_fradiusY(10.f)
	, m_colorMouseOn(D2D1::ColorF::White)
	, m_colorNormal(D2D1::ColorF::White)
	, m_fMouseOnAlpha(1.0f)
	, m_fNormalAlpha(1.0f)
{

}

CPanelUI::~CPanelUI()
{

}

void CPanelUI::update()
{
	
}

void CPanelUI::MouseOn()
{

}

void CPanelUI::MouseLbtnDown()
{

	//�г� UI�� �巡���ؼ� �̵� �����ϰ� �ؾ���. 
	//���� ������ ����� �ؼ� �װ� ��ǥ�� ����ϰ�, ������ ��ǥ�� ����Ͽ� �Ȱ��� �Ÿ���ŭ �̵���Ű�� �Ǵ� ��. 
	if (m_bCanMove) {
		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnUp()
{
}

void CPanelUI::render(ID2D1HwndRenderTarget* _pRender)
{

	Vec2 vPos = GetFinalPos();
	//vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(
			vPos.x - vScale.x / 2.f,
			vPos.y - vScale.y / 2.f,
			vPos.x + vScale.x / 2.f,
			vPos.y + vScale.y / 2.f), // �簢���� ��ǥ (left, top, right, bottom)
		m_fradiusX, // X�� ������ (radiusX)
		m_fradiusY  // Y�� ������ (radiusY)
	);

	ID2D1SolidColorBrush* pBrush = nullptr;


	HRESULT hr = _pRender->CreateSolidColorBrush(
		m_bMouseOn
		? D2D1::ColorF(m_colorMouseOn.r, m_colorMouseOn.g, m_colorMouseOn.b, m_fMouseOnAlpha)
		: D2D1::ColorF(m_colorNormal.r, m_colorNormal.g, m_colorNormal.b, m_fNormalAlpha),
		&pBrush
	);

	if (SUCCEEDED(hr))
	{
		_pRender->FillRoundedRectangle(roundedRect, pBrush);
		pBrush->Release();
	}
	component_render(_pRender);
	render_child(_pRender);
	//CUI::render(_pRender);
}