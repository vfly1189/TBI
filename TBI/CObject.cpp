#include "global.h"
#include "CObject.h"
#include "CTimeMgr.h"
#include "Direct2DMgr.h"
#include "CPathMgr.h"

#include "CTextUI.h"

CObject::CObject()
	: m_vPos{}
	, m_ObjType(GROUP_TYPE::END)
	, m_vScale{}
	, m_bAlive(true)
	, m_bEnable(true)
{
	m_vRenderScale = m_vScale;
}

CObject::CObject(const CObject& _origin)
	: m_ObjName(_origin.m_ObjName)
	, m_ObjType(_origin.m_ObjType)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_bAlive(true)
	, m_bEnable(true)
{
	m_vRenderScale = m_vScale;
	

}

CObject::~CObject() {

}

void CObject::update()
{

}

void CObject::finalupdate()
{
	
}

void CObject::render(ID2D1HwndRenderTarget* _pRender)
{
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);

	float left = vRenderPos.x - m_vRenderScale.x / 2.f;
	float top = vRenderPos.y - m_vRenderScale.y / 2.f;
	float right = vRenderPos.x + m_vRenderScale.x / 2.f;
	float bottom = vRenderPos.y + m_vRenderScale.y / 2.f;

	D2D1_RECT_F rect = D2D1::RectF(left, top, right, bottom);

	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White), &pBrush);

	if (SUCCEEDED(hr))
	{
		_pRender->DrawRectangle(rect, pBrush);
		pBrush->Release();
	}

	component_render(_pRender);
}


void CObject::component_render(ID2D1HwndRenderTarget* _pRender)
{
	if (m_pTextUI != nullptr)
		m_pTextUI->render(_pRender);
}


void CObject::SetUIText(const std::wstring& text)
{
	if (!m_pTextUI) CreateTextUI();
	m_pTextUI->SetText(text);
}

void CObject::CreateTextUI()
{
	m_pTextUI = new CTextUI;
	m_pTextUI->m_pOwner = this;
}