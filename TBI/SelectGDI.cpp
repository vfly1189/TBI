#include "global.h"
#include "SelectGDI.h"

#include "CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType)
	: m_dc(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	
	//�浹ü�� �׵θ� ���θ� �׷����� ��.
//hollowBrush -> ���� �귯��. �׵θ���.
	HPEN hPen = CCore::GetInstance()->GetPen(_ePenType);
	m_hDefaultPen = (HPEN)SelectObject(_dc, hPen);
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType)
	: m_dc(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	
	HBRUSH hHollowBrush = CCore::GetInstance()->GetBrush(_eBrushType);
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hHollowBrush);
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_dc, m_hDefaultPen);
	SelectObject(m_dc, m_hDefaultBrush);

	DeleteObject(m_hDefaultPen);
	DeleteObject(m_hDefaultBrush);
}
