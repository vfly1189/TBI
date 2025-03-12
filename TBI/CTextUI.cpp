#include "global.h"
#include "CTextUI.h"

#include "CObject.h"

#include "CCamera.h"
#include "CFontMgr.h"

CTextUI::CTextUI()
{
	
}

CTextUI::~CTextUI()
{

}

void CTextUI::render(ID2D1HwndRenderTarget* _pRender)
{
	if (!m_pOwner || m_strText.empty()) return;

	Vec2 vPos = CCamera::GetInstance()->GetRenderPos(m_pOwner->GetPos());

	D2D1_RECT_F rect = D2D1::RectF(
		vPos.x - 100.f, vPos.y - 50.f,
		vPos.x + 100.f, vPos.y + 50.f);

	ID2D1SolidColorBrush* brush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

	if (SUCCEEDED(hr))
	{
		_pRender->DrawText(
			m_strText.c_str(),
			static_cast<UINT32>(m_strText.length()),
			CFontMgr::GetInstance()->GetTextFormat(),
			&rect,
			brush);

		brush->Release();
	}
}