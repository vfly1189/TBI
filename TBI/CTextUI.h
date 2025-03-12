#pragma once

class CObject;

class CTextUI
{
private:
	CObject* m_pOwner;
	std::wstring	m_strText;	// ����� �ؽ�Ʈ

public:
	void SetText(const std::wstring& _text) { m_strText = _text; }
	void render(ID2D1HwndRenderTarget* _pRender);

public:
	CTextUI();
	~CTextUI();

	friend class CObject;
};
