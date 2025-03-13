#pragma once

class CObject;

class CTextUI
{
private:
	CObject*			m_pOwner;		//소유하고 잇는 오브젝트 주소

	wstring				m_strText;	// 출력할 텍스트
	float				m_fFontSize;
	D2D1::ColorF		m_colorText;

	bool				m_bdrawOutline;
	float				m_fOutlineThickness = 1.f;
	D2D1::ColorF		m_colorOutline;

	Vec2				m_vOffsetLT;	//그려질 위치 좌상단
	Vec2				m_vOffsetRB;	//그려질 위치 우하단

	int					m_iHorizontal; // 0 : 중앙, 1 : 좌측 , 2 : 우측

public:
	void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }

	void SetText(const std::wstring& _text) { m_strText = _text; }
	void SetFontSize(float _fontSize) { m_fFontSize = _fontSize; }
	void SetTextColor(D2D1::ColorF _colorText) { m_colorText = _colorText; }

	void SetDrawOutline(bool _bOutline) { m_bdrawOutline = _bOutline; }
	void SetOutlineThickness(float _outlineThickness) { m_fOutlineThickness = _outlineThickness; }
	void SetOutlineColor(D2D1::ColorF _colorOutline) { m_colorOutline = _colorOutline; }

	void SetOffsetLT(Vec2 _offsetLT) { m_vOffsetLT = _offsetLT; }
	void SetOffsetRB(Vec2 _offsetRB) { m_vOffsetRB = _offsetRB; }

	void SetHorizontal(int _horizontal) { m_iHorizontal = _horizontal; }

public:
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	CTextUI();
	~CTextUI();

	friend class CObject;
};
