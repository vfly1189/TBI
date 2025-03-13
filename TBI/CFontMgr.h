#pragma once

class CFontMgr
{
    SINGLE(CFontMgr)

private:
    IDWriteFactory* m_pDWriteFactory; // DirectWrite ���丮
    IDWriteTextFormat* m_pTextFormat; // �ؽ�Ʈ ����

public:
    HRESULT LoadSystemFont(const std::wstring& fontFamilyName, float fontSize);
    IDWriteTextFormat* GetTextFormat(const std::wstring& fontFamilyName, float fontSize);

public:
    void init();


public:
    CFontMgr(const CFontMgr&) = delete;
    void operator=(const CFontMgr&) = delete;
};
