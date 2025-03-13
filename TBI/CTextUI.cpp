#include "global.h"
#include "CTextUI.h"

#include "CObject.h"

#include "CCamera.h"
#include "Direct2DMgr.h"
#include "CFontMgr.h"

CTextUI::CTextUI()
	: m_colorText(D2D1::ColorF::Black)
	, m_colorOutline(D2D1::ColorF::White)
{
	
}

CTextUI::~CTextUI()
{

}

void CTextUI::render(ID2D1HwndRenderTarget* _pRender)
{
	if (!m_pOwner || m_strText.empty()) return;

	auto d2dManager = Direct2DMgr::GetInstance();

	Vec2 vPos = CCamera::GetInstance()->GetRenderPos(m_pOwner->GetFinalPos());

	
	ID2D1SolidColorBrush* brush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

	if (SUCCEEDED(hr))
	{
		d2dManager->RenderTextWithOutline(
			m_strText,
			D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y),
			m_fFontSize,
			m_colorText,
			m_colorOutline,
			m_fOutlineThickness,
			m_iHorizontal
		);
		brush->Release();
	}
}